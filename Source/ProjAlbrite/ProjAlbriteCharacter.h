// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Interfaces/IDamageable.h"
#include "Enums/GameEnums.h"
#include "AlbriteAttributeSet.generated.h"
#include "AbilitySystemInterface.h"
#include "Logging/LogMacros.h"
#include "ProjAlbriteCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;
class UInputMappingContext;
class UInputAction;
struct FInputActionValue;

class UAlbriteBaseGameplayAbility;

DECLARE_LOG_CATEGORY_EXTERN(LogTemplateCharacter, Log, All);

UCLASS(config=Game)
class AProjAlbriteCharacter : public ACharacter, public IAbilitySystemInterface, public IIDamageable
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
	
public:
	AProjAlbriteCharacter();
	
	/** Gameplay Ability System */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="GAS", meta = (AllowPrivateAccess="true"))
	class UAlbriteAbilitySystemComponent* AbilitySystemComponent;
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	
	/** Gameplay Attributes */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="GAS", meta = (AllowPrivateAccess="true"))
	TObjectPtr<UAlbriteAttributeSet> AttributeSet;

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
	
protected:
	/** Called for Begin play */
	virtual void BeginPlay() override;
	
	/** Called for movement input */
	void Move(const FInputActionValue& Value);

	/** Called for looking input */
	void Look(const FInputActionValue& Value);
			

protected:

	virtual void NotifyControllerChanged() override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

public:
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }
};

