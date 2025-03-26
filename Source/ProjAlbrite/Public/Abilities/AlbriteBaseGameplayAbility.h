// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "Enums/GameEnums.h"
#include "AlbriteBaseGameplayAbility.generated.h"

/**
 * The base class for Game abilities to allow customized control for future upgrades
 * Author: Burlin
 */
UCLASS()
class PROJALBRITE_API UAlbriteBaseGameplayAbility : public UGameplayAbility
{
	GENERATED_BODY()

public:
	UAlbriteBaseGameplayAbility();

	// Abilities will activate upon input press
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category="Abilities")
	EAbilityInputID AbilityInputId;
	
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category="Abilities")
	bool bDisabledMovement;
	
	// Override Activate Ability
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

	// Override Can Activate Ability
	virtual bool CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags, const FGameplayTagContainer* TargetTags, FGameplayTagContainer* OptionalRelevantTags) const override;
};
