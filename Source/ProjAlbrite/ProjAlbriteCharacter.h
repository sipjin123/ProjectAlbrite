// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Interfaces/IDamageable.h"
#include "Enums/GameEnums.h"
#include "AbilitySystemInterface.h"
#include "CombatUnitWidget.h"
#include "Components/WidgetComponent.h"
#include "ActorComponents/AlbriteAbilitySystemComponent.h"
#include "Enums/StatusTypes.h"
#include "Interfaces/IAlbriteCharacter.h"
#include "Logging/LogMacros.h"
#include "Stats/AlbriteAttributeSet.h"
#include "Widgets/PlayerUIWidget.h"
#include "ProjAlbriteCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;
class UInputMappingContext;
class UInputAction;
struct FInputActionValue;

class UAlbriteBaseGameplayAbility;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FHitTarget, FVector, Location, ECombatElementType, CEtype);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnAimTriggered, bool, IsAim);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FInvulnerableChange, bool, InvulnerableValue);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FStunChange, bool, StunValue);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FAttributeChange, int, AttributeValue);
DECLARE_LOG_CATEGORY_EXTERN(LogTemplateCharacter, Log, All);

UCLASS(config=Game)
class AProjAlbriteCharacter : public ACharacter, public IAbilitySystemInterface, public IIDamageable, public IIAlbriteCharacter
{
	GENERATED_BODY()

	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FollowCamera;
	
	/** MappingContext */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputMappingContext* DefaultMappingContext;

	/** Jump Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* JumpAction;

	/** Move Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* MoveAction;

	/** Look Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* LookAction;

	/** Attack Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* AttackAction;
	
	/** Defense Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* DefenseAction;
	
	/** Dash Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* DashAction;
	
	/** Special Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* SpecialAction;
	
	/** Ultimate Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* UltimateAction;
	
	/** Aim Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* AimAction;
	
public:
	AProjAlbriteCharacter();
	
	/** Gameplay Ability System */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="GAS", meta = (AllowPrivateAccess="true"))
	class UAlbriteAbilitySystemComponent* AbilitySystemComponent;
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override { return AbilitySystemComponent; };
	
	/** Gameplay Attributes */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="GAS", meta = (AllowPrivateAccess="true"))
	const class UAlbriteAttributeSet* AttributeSet;
	
	/** Gameplay Array of Abilities */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="GAS")
	TArray<TSubclassOf<UAlbriteBaseGameplayAbility>> GameplayAbilities;

	/** Initializes Abilities to player */
	UFUNCTION(BlueprintCallable, Category="GAS")
	void InitializeAbilities();

	/** Checks if abilities have been initialized */
	bool bAbilitiesInitialized = false;

	/** Give Abilities to player */
	void GrantAbility(TSubclassOf<UAlbriteBaseGameplayAbility> AbilityClass, EAbilityInputID InputId) const;

	/** Used when client requests an ability to be casted using input id */
	UFUNCTION(BlueprintCallable)
	void OnAbilityInputPressed(EAbilityInputID InputID);

	/** Server side activation of ability based on input id */
	UFUNCTION(Server, Reliable, WithValidation)
	void Server_ActivateAbility(EAbilityInputID InputID);
	
	/** Server side Hitscan checker to prevent client side cheating **/
	UFUNCTION(Server, Reliable, WithValidation)
	void Server_RequestHitScan();

	/** The hitscan origin adjusted in viewport **/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "FPS")
	UArrowComponent* FPSArrowComponent;
	
protected:
	/** Called for Begin play */
	virtual void BeginPlay() override;
	
	/** Called for movement input */
	void Move(const FInputActionValue& Value);

	/** Called for looking input */
	void Look(const FInputActionValue& Value);

protected:
	// Overrides
	virtual void NotifyControllerChanged() override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual void PossessedBy(AController* NewController) override;
	virtual void OnRep_PlayerState() override;
	
	/** Reference to the actor component */
	UPROPERTY(EditAnywhere, Category = "Components")
	class UStatusActorComponent* StatusActorComponent;

	UPROPERTY(EditAnywhere, Category = "Components")
	class UVFXActorComponent* VFXActorComponent;
	
public:
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }

	/** Retrieves Character Movement Component **/
	virtual UCharacterMovementComponent* GetCharacterMovementComponent_Implementation() override { return GetCharacterMovement(); };

	/** Retrieves The base Attribute for Health **/
	virtual float OnGetCurrentHealth_Implementation() override { return AttributeSet ? AttributeSet->Health.GetBaseValue() : 0.1f; };
	
	/** Retrieves The base Attribute for Max Health **/
	virtual float OnGetMaxHealth_Implementation() override { return AttributeSet ? AttributeSet->MaxHealth.GetBaseValue() : 0.1f; };

	/** This callback can be used by the UI. **/
	UPROPERTY(BlueprintAssignable, Category = "Attribute callbacks")
	FAttributeChange OnHealthChange;
	void OnHealthUpdated(const FOnAttributeChangeData& OnAttributeChangeData) const;
	
	UPROPERTY(BlueprintAssignable, Category = "Attribute callbacks")
	FAttributeChange OnShieldChange;
	void OnShieldUpdated(const FOnAttributeChangeData& OnAttributeChangeData) const;

	/** The statuses this unit has at the moment **/
	UPROPERTY(BlueprintReadWrite, Category="Combat")
	TSet<EStatusType> Statuses;

	/** The determines if the unit is aiming **/
	UPROPERTY(BlueprintReadWrite, Replicated, ReplicatedUsing = OnRep_IsAiming, Category="Combat")
	bool bIsAiming;
	UFUNCTION()
	void OnRep_IsAiming();
	
	/** Handles aim control states **/
	UPROPERTY(BlueprintAssignable, Category="Combat")
	FOnAimTriggered OnAimTriggered;
	void OnAimTriggerPressed() { OnAimTriggered.Broadcast(true); };
	void OnAimTriggerReleased() { OnAimTriggered.Broadcast(false); };
	
	/** Status check **/
	bool DoesStatusExist(EStatusType StatusType) const { return Statuses.Contains(StatusType); };

	void ApplyDamageToTarget(AActor* Target);

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TSubclassOf<UGameplayEffect> BulletDamageEffect;
	
	/** Signals that a projectile hits a target **/
	UPROPERTY(BlueprintAssignable, Category="Combat")
	FHitTarget HitTarget;
	
	/** Events that are binded in blueprints to listen to **/
	UPROPERTY(BlueprintAssignable, Category = "Stat callbacks")
	FInvulnerableChange OnInvulnerableChange;
	UPROPERTY(BlueprintAssignable, Category = "Stat callbacks")
	FStunChange OnStunChange;

	/** Function that handles status states based on tags **/
	void OnInvulnerableTagChanged(FGameplayTag GameplayTag, int NewVal);
	void OnStunTagChanged(FGameplayTag GameplayTag, int NewVal);

	/** Retrieves status states based on tags **/
	virtual bool IsStunned_Implementation() override { return AbilitySystemComponent ? AbilitySystemComponent->ComponentHasTag(FName("Status.Stun")) : false; };
	virtual bool IsInvulnerable_Implementation () override { return AbilitySystemComponent ? AbilitySystemComponent->ComponentHasTag(FName("Status.Invulnerable")) : false; };
	virtual bool IsUnitDead_Implementation() override { return AttributeSet ? AttributeSet->Health.GetBaseValue() <= 0 : false; };

	/** Widget class for displaying UI on the character screen **/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	TSubclassOf<UPlayerUIWidget> PlayerUIWidgetClass;

	/** Widget direct reference for displaying UI on the character screen **/
	UPROPERTY(BlueprintReadWrite, Category = "UI")
	UPlayerUIWidget* PlayerUIWidget;

	/** Widget Component for displaying UI above the character **/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UI", meta = (AllowPrivateAccess = "true"))
	UWidgetComponent* CombatWidgetComponent;
	
	/** Widget direct reference for displaying UI above the character **/
	UPROPERTY(BlueprintReadWrite, Category = "UI")
	UCombatUnitWidget* CombatWidget;
};

