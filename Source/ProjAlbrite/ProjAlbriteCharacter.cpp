// Copyright Epic Games, Inc. All Rights Reserved.

#include "ProjAlbriteCharacter.h"
#include "AbilitySystemComponent.h"
#include "Engine/LocalPlayer.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/Controller.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Net/UnrealNetwork.h"
#include "InputActionValue.h"
#include "Abilities/AlbriteBaseGameplayAbility.h"
#include "ActorComponents/AlbriteAbilitySystemComponent.h"
#include "ActorComponents/StatusActorComponent.h"
#include "ActorComponents/VFXActorComponent.h"
#include "Components/ArrowComponent.h"
#include "Components/WidgetComponent.h"
#include "Core/AlbriteGameInstance.h"
#include "Enums/GameEnums.h"

DEFINE_LOG_CATEGORY(LogTemplateCharacter);

//////////////////////////////////////////////////////////////////////////
// AProjAlbriteCharacter

AProjAlbriteCharacter::AProjAlbriteCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);
		
	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f); // ...at this rotation rate

	// Note: For faster iteration times these variables, and many more, can be tweaked in the Character Blueprint
	// instead of recompiling to adjust them
	GetCharacterMovement()->JumpZVelocity = 700.f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = 500.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;
	GetCharacterMovement()->BrakingDecelerationFalling = 1500.0f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 400.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named ThirdPersonCharacter (to avoid direct content references in C++)

	AbilitySystemComponent = CreateDefaultSubobject<UAlbriteAbilitySystemComponent>(TEXT("AbilitySystemComponent"));
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Minimal);
	
	AttributeSet = CreateDefaultSubobject<UAlbriteAttributeSet>(TEXT("Attributes"));
	
	// Create and attach the widget component
	CombatWidgetComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("CombatWidget"));
	CombatWidgetComponent->SetupAttachment(RootComponent); // Attach to the character
	CombatWidgetComponent->SetWidgetSpace(EWidgetSpace::World); // Set to World Space
	CombatWidgetComponent->SetDrawSize(FVector2D(100, 10)); // Set UI size
	CombatWidgetComponent->SetRelativeLocation(FVector(0, 0, 100)); // Position above head
	
	FPSArrowComponent = CreateDefaultSubobject<UArrowComponent>(TEXT("FPSArrowComponent"));
	FPSArrowComponent->SetupAttachment(CameraBoom);
}

void AProjAlbriteCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME_CONDITION_NOTIFY(AProjAlbriteCharacter, bIsAiming, COND_None, REPNOTIFY_OnChanged);
	DOREPLIFETIME_CONDITION_NOTIFY(AProjAlbriteCharacter, CurrentElement, COND_None, REPNOTIFY_OnChanged);
}

void AProjAlbriteCharacter::InitializeAbilities()
{
	check (AbilitySystemComponent);
	if (HasAuthority() && !bAbilitiesInitialized && GameplayAbilities.Num() > 0)
	{
		// Grant active abilities only for the server, retrieve the Ability Input Id assigned to the Ability
		for (TSubclassOf<UAlbriteBaseGameplayAbility>& StartupAbilityEntry : GameplayAbilities)
		{
			GrantAbility(StartupAbilityEntry, StartupAbilityEntry.GetDefaultObject()->AbilityInputId);
		}
		bAbilitiesInitialized = true;
	}
}

void AProjAlbriteCharacter::GrantAbility(TSubclassOf<UAlbriteBaseGameplayAbility> AbilityClass, EAbilityInputID InputId) const
{
	if (HasAuthority() && AbilitySystemComponent && AbilityClass)
	{
		// Give ability to player and assign Input Id that links to this ability
		const FGameplayAbilitySpec AbilitySpec(AbilityClass, 1, static_cast<int32>(InputId));
		AbilitySystemComponent->GiveAbility(AbilitySpec);
	}
}

void AProjAlbriteCharacter::OnAbilityInputPressed(EAbilityInputID InputID)
{
	// Request ability cast from server
	if (InputID == EAbilityInputID::Attack && bIsAiming)
	{
		Server_RequestHitScan();
	}
	else
	{
		Server_ActivateAbility(InputID);
	}
}

void AProjAlbriteCharacter::Server_ActivateAbility_Implementation(EAbilityInputID InputID)
{
	if (AbilitySystemComponent)
	{
		for (FGameplayAbilitySpec& Spec : AbilitySystemComponent->GetActivatableAbilities())
		{
			// Server will try to cast ability that uses Input Id
			if (Spec.InputID == static_cast<int32>(InputID))
			{
				AbilitySystemComponent->TryActivateAbility(Spec.Handle);
				break;
			}
		}
	}
}

bool AProjAlbriteCharacter::Server_ActivateAbility_Validate(EAbilityInputID InputID)
{
	// To Do: Add validation checkers
	return true;
}

void AProjAlbriteCharacter::Server_RequestHitScan_Implementation()
{
	FHitResult OutHitResult;
	constexpr float TraceDistance = 5000.f;
	
	// Get the camera location and forward direction
	FVector StartLocation;
	FRotator ViewRotation;
	GetController()->GetPlayerViewPoint(StartLocation, ViewRotation);

	FVector EndLocation = StartLocation + (ViewRotation.Vector() * TraceDistance);

	// Set up collision query parameters
	FCollisionQueryParams TraceParams;
	TraceParams.AddIgnoredActor(this); // Ignore the character

	// Perform the line trace
	bool bHit = GetWorld()->LineTraceSingleByChannel(
		OutHitResult, 
		FPSArrowComponent->GetComponentLocation(), 
		EndLocation,
		ECC_Camera,//ECC_Visibility,  // Collision channel
		TraceParams
	);

	if (OutHitResult.GetActor())
	{
		ApplyDamageToTarget(OutHitResult.GetActor(), AttributeSet->GetDamage());
	}
	
	HitTarget.Broadcast(OutHitResult.ImpactPoint, ECombatElementType::None);

	// Debug visualization (remove in production)
	// DrawDebugLine(GetWorld(), StartLocation, EndLocation, FColor::Red, false, 1.0f, 0, .1f);
}

bool AProjAlbriteCharacter::Server_RequestHitScan_Validate()
{
	// To Do: Add validation checkers
	return true;
}

void AProjAlbriteCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	if (HasAuthority())  
	{
		GetMesh()->GetAnimInstance()->SetRootMotionMode(ERootMotionMode::RootMotionFromMontagesOnly);
	}
	
	StatusActorComponent = FindComponentByClass<UStatusActorComponent>();
	VFXActorComponent = FindComponentByClass<UVFXActorComponent>();

	check(StatusActorComponent);
	check(VFXActorComponent);
	
	if (IsValid(AbilitySystemComponent))
	{
		AttributeSet = AbilitySystemComponent->GetSet<UAlbriteAttributeSet>();
		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
			UAlbriteAttributeSet::GetHealthAttribute()).AddUObject(this, &AProjAlbriteCharacter::OnHealthUpdated);
		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
			UAlbriteAttributeSet::GetShieldAttribute()).AddUObject(this, &AProjAlbriteCharacter::OnShieldUpdated);

		AbilitySystemComponent->RegisterGameplayTagEvent(
			FGameplayTag::RequestGameplayTag(FName("Status.Invulnerable")),
			EGameplayTagEventType::NewOrRemoved)
			.AddUObject(this, &AProjAlbriteCharacter::OnInvulnerableTagChanged);
		
		AbilitySystemComponent->RegisterGameplayTagEvent(
			FGameplayTag::RequestGameplayTag(FName("Status.Stun")),
			EGameplayTagEventType::NewOrRemoved)
			.AddUObject(this, &AProjAlbriteCharacter::OnStunTagChanged);

		// Imbue elements and apply visual indicators in blueprint
		ECombatElementType Ice = ECombatElementType::Ice;
		AbilitySystemComponent->RegisterGameplayTagEvent(
			FGameplayTag::RequestGameplayTag(FName("Imbue.Ice")),
			EGameplayTagEventType::NewOrRemoved)
			.AddLambda([this, Ice](const FGameplayTag Tag, int32 NewCount)
			{
				OnStatusChanged(Ice, NewCount);
			});
		ECombatElementType Fire = ECombatElementType::Fire;
		AbilitySystemComponent->RegisterGameplayTagEvent(
			FGameplayTag::RequestGameplayTag(FName("Imbue.Fire")),
			EGameplayTagEventType::NewOrRemoved)
			.AddLambda([this, Fire](const FGameplayTag Tag, int32 NewCount)
			{
				OnStatusChanged(Fire, NewCount);
			});
		ECombatElementType Lightning = ECombatElementType::Lightning;
		AbilitySystemComponent->RegisterGameplayTagEvent(
			FGameplayTag::RequestGameplayTag(FName("Imbue.Lightning")),
			EGameplayTagEventType::NewOrRemoved)
			.AddLambda([this, Lightning](const FGameplayTag Tag, int32 NewCount)
			{
				OnStatusChanged(Lightning, NewCount);
			});
	}

	UAlbriteGameInstance* GI = Cast<UAlbriteGameInstance>(GetGameInstance());
	if (GI && HasAuthority())
	{
		GI->RegisterCharacter(this);
	}
	CombatWidget = Cast<UCombatUnitWidget>(CombatWidgetComponent->GetWidget());
	
	if (CombatWidget && GetLocalRole() == ROLE_AutonomousProxy)
	{
		CombatWidget->SetVisibility(ESlateVisibility::Hidden);
	}
}

//////////////////////////////////////////////////////////////////////////
// Input

void AProjAlbriteCharacter::NotifyControllerChanged()
{
	Super::NotifyControllerChanged();

	// Add Input Mapping Context
	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}
}

void AProjAlbriteCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent)) {
		
		// Jumping
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ACharacter::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);

		// Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AProjAlbriteCharacter::Move);

		// Looking
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AProjAlbriteCharacter::Look);

		if (!AbilitySystemComponent) return;

		// Bind Ability Activation to Input
		AbilitySystemComponent->BindAbilityActivationToInputComponent(EnhancedInputComponent, 
			FGameplayAbilityInputBinds(
				TEXT("Confirm"), TEXT("Cancel"),
				"/Script/ProjAlbrite.EAbilityInputID", 
				static_cast<int32>(EAbilityInputID::None), 
				static_cast<int32>(EAbilityInputID::None)
			)
		);

		// Bind Action to ability input pressed function
		EnhancedInputComponent->BindAction(AttackAction, ETriggerEvent::Started, this, &AProjAlbriteCharacter::OnAbilityInputPressed, EAbilityInputID::Attack);
		EnhancedInputComponent->BindAction(DashAction, ETriggerEvent::Started, this, &AProjAlbriteCharacter::OnAbilityInputPressed, EAbilityInputID::Dash);
		EnhancedInputComponent->BindAction(DefenseAction, ETriggerEvent::Started, this, &AProjAlbriteCharacter::OnAbilityInputPressed, EAbilityInputID::Defense);
		EnhancedInputComponent->BindAction(SpecialAction, ETriggerEvent::Started, this, &AProjAlbriteCharacter::OnAbilityInputPressed, EAbilityInputID::Special);
		EnhancedInputComponent->BindAction(UltimateAction, ETriggerEvent::Started, this, &AProjAlbriteCharacter::OnAbilityInputPressed, EAbilityInputID::Ultimate);
		EnhancedInputComponent->BindAction(AimAction, ETriggerEvent::Started, this, &AProjAlbriteCharacter::OnAimTriggerPressed);
		EnhancedInputComponent->BindAction(AimAction, ETriggerEvent::Completed, this, &AProjAlbriteCharacter::OnAimTriggerReleased);
	}
	else
	{
		UE_LOG(LogTemplateCharacter, Error, TEXT("'%s' Failed to find an Enhanced Input component! This template is built to use the Enhanced Input system. If you intend to use the legacy system, then you will need to update this C++ file."), *GetNameSafe(this));
	}
}

void AProjAlbriteCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);
}

void AProjAlbriteCharacter::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();

	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (GetLocalRole() == ROLE_AutonomousProxy)
		{
			PlayerUIWidget = CreateWidget<UPlayerUIWidget>(PlayerController, PlayerUIWidgetClass);
			if (PlayerUIWidget)
			{
				PlayerUIWidget->AddToViewport();
			}
		}
	}
}

void AProjAlbriteCharacter::OnHealthUpdated(const FOnAttributeChangeData& OnAttributeChangeData) const
{
	OnHealthChange.Broadcast(OnAttributeChangeData.NewValue);
}

void AProjAlbriteCharacter::OnShieldUpdated(const FOnAttributeChangeData& OnAttributeChangeData) const
{
	OnShieldChange.Broadcast(OnAttributeChangeData.NewValue);
}

void AProjAlbriteCharacter::OnRep_CurrentElement()
{
	OnImbueElement.Broadcast(CurrentElement);
}

void AProjAlbriteCharacter::OnStatusChanged(ECombatElementType ElementType, int32 NewCount)
{
	FString ElementString = UEnum::GetDisplayValueAsText(ElementType).ToString();

	if (NewCount > 0 && HasAuthority())
	{
		CurrentElement = ElementType;
	}
}

void AProjAlbriteCharacter::OnRep_IsAiming()
{
}

void AProjAlbriteCharacter::Move(const FInputActionValue& Value)
{
	// input is a Vector2D
	const FVector2D MovementVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	
		// get right vector 
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		// add movement 
		AddMovementInput(ForwardDirection, MovementVector.Y);
		AddMovementInput(RightDirection, MovementVector.X);
	}
}

void AProjAlbriteCharacter::Look(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// add yaw and pitch input to controller
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}
}

void AProjAlbriteCharacter::ApplyDamageToTarget(AActor* Target, float Damage)
{
	if (!Target || !BulletDamageEffect) return;

	// Get the target's Ability System Component
	UAbilitySystemComponent* TargetASC = Target->FindComponentByClass<UAbilitySystemComponent>();
	if (!TargetASC) return;

	// Create an EffectSpecHandle from the EffectClass
	FGameplayEffectContextHandle EffectContext = TargetASC->MakeEffectContext();
	EffectContext.AddInstigator(GetOwner(), nullptr);
	
	float DamageValue = Damage;
	
	// Apply flinch effect if possible
	if (Target->GetClass()->ImplementsInterface(UIDamageable::StaticClass()))
	{
		IIDamageable::Execute_ApplyFlinch(Target);
		if (!IIDamageable::Execute_IsUnitDead(Target))
		{
			FGameplayEffectSpecHandle EffectSpecHandle = TargetASC->MakeOutgoingSpec(BulletDamageEffect, 1, EffectContext);
			// Set the custom magnitude value before applying the effect
			EffectSpecHandle.Data->SetSetByCallerMagnitude(FGameplayTag::RequestGameplayTag(FName("Combat.Damage")), DamageValue);

			if (EffectSpecHandle.IsValid())
			{
				// Apply the effect to the target
				TargetASC->ApplyGameplayEffectSpecToSelf(*EffectSpecHandle.Data.Get());
			}
		}
	}
}

void AProjAlbriteCharacter::OnInvulnerableTagChanged(FGameplayTag GameplayTag, int NewVal)
{
	if (HasAuthority())
	{
		// To Do: Server Logic Here
	}
	else
	{
		// Trigger VFX on Client side
		OnInvulnerableChange.Broadcast(NewVal == 0 ? false : true);
	}
}

void AProjAlbriteCharacter::OnStunTagChanged(FGameplayTag GameplayTag, int NewVal)
{
	if (HasAuthority())
	{
		// To Do: Server Logic Here
	}
	else
	{
		// Trigger VFX on Client side
		OnStunChange.Broadcast(NewVal == 0 ? false : true);
	}
}

void AProjAlbriteCharacter::ApplyCooldown_Implementation(TSubclassOf<UGameplayEffect> EffectClass, EAbilityInputID AbilityUsed)
{
	// Only apply on the server
	if (EffectClass && HasAuthority())
	{
		ServerApplyCooldown(EffectClass, AbilityUsed);
	}
}

void AProjAlbriteCharacter::ServerApplyCooldown_Implementation(TSubclassOf<UGameplayEffect> EffectClass, EAbilityInputID AbilityUsed)
{
	if (EffectClass && HasAuthority())
	{
		// Apply effect
		const FGameplayEffectSpecHandle SpecHandle = AbilitySystemComponent->MakeOutgoingSpec(EffectClass, 1.f, AbilitySystemComponent->MakeEffectContext());
		if (SpecHandle.IsValid())
		{
			const FActiveGameplayEffectHandle ActiveEffectHandle = AbilitySystemComponent->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());

			// Get duration from the applied effect
			const FActiveGameplayEffect* ActiveEffect = AbilitySystemComponent->GetActiveGameplayEffect(ActiveEffectHandle);
			if (ActiveEffect)
			{
				float Duration = ActiveEffect->GetDuration();
				ClientApplyCooldown(Duration, AbilityUsed);
			}
		}
	}
}

void AProjAlbriteCharacter::ClientApplyCooldown_Implementation(float Duration, EAbilityInputID AbilityUsed)
{
	CooldownTriggered.Broadcast(Duration, AbilityUsed);
}
