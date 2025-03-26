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

	AbilitySystemComponent = CreateDefaultSubobject<UAbilitySystemComponent>(TEXT("AbilitySystemComponent"));
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Minimal);
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
	Server_ActivateAbility(InputID);
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
	return true;
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
	}
	else
	{
		UE_LOG(LogTemplateCharacter, Error, TEXT("'%s' Failed to find an Enhanced Input component! This template is built to use the Enhanced Input system. If you intend to use the legacy system, then you will need to update this C++ file."), *GetNameSafe(this));
	}
}

void AProjAlbriteCharacter::Move(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D MovementVector = Value.Get<FVector2D>();

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
