// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "AlbriteAbilitySystemComponent.generated.h"

/**
 * 
 */
UCLASS(BlueprintType)
class PROJALBRITE_API UAlbriteAbilitySystemComponent : public UAbilitySystemComponent
{
	GENERATED_BODY()
	
	virtual void NotifyAbilityActivated(const FGameplayAbilitySpecHandle Handle, UGameplayAbility* Ability) override;

	// Custom code removing tags within a category
	UFUNCTION(BlueprintCallable)
	void RemoveTagsInCategory(UAbilitySystemComponent* AbilitySystemComponent, const FGameplayTag ParentTag);

	UFUNCTION(NetMulticast, Reliable)
	void Multicast_NotifyTagsRemoved(UAbilitySystemComponent* AbilitySystemComponent, const TArray<FGameplayTag>& TagsToRemove);

	UFUNCTION(BlueprintCallable)
	void LogTags(UAbilitySystemComponent* AbilitySystemComponent);
};
