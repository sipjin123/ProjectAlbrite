// Fill out your copyright notice in the Description page of Project Settings.


#include "ActorComponents/AlbriteAbilitySystemComponent.h"

void UAlbriteAbilitySystemComponent::NotifyAbilityActivated(const FGameplayAbilitySpecHandle Handle,
	UGameplayAbility* Ability)
{
	Super::NotifyAbilityActivated(Handle, Ability);
}

void UAlbriteAbilitySystemComponent::RemoveTagsInCategory(UAbilitySystemComponent* AbilitySystemComponent,
	const FGameplayTag ParentTag)
{
	if (!AbilitySystemComponent) return;

	FGameplayTagContainer OwnedTags;
	AbilitySystemComponent->GetOwnedGameplayTags(OwnedTags);

	TArray<FGameplayTag> TagsToRemove;
	for (const FGameplayTag& Tag : OwnedTags)
	{
		if (Tag.MatchesTag(ParentTag))  // Check if the tag is within the subcategory
		{
			TagsToRemove.Add(Tag);
		}
	}

	for (const FGameplayTag& Tag : TagsToRemove)
	{
		AbilitySystemComponent->RemoveLooseGameplayTag(Tag);
		
		// Notify all clients to sync tag removal
		Multicast_NotifyTagsRemoved(AbilitySystemComponent, TagsToRemove);
	}
}

void UAlbriteAbilitySystemComponent::LogTags(UAbilitySystemComponent* AbilitySystemComponent)
{
	if (!AbilitySystemComponent) return;

	FGameplayTagContainer OwnedTags;
	AbilitySystemComponent->GetOwnedGameplayTags(OwnedTags);

	TArray<FGameplayTag> TagsToRemove;
	for (const FGameplayTag& Tag : OwnedTags)
	{
		UE_LOG(LogTemp, Warning, TEXT("Tag owned is: %s"), *Tag.ToString());
	}
}

void UAlbriteAbilitySystemComponent::Server_ApplyCooldownByTag_Implementation(const FGameplayTag TagToUse)
{
	Client_ApplyCooldownByTag_Implementation(TagToUse);
}

void UAlbriteAbilitySystemComponent::Client_ApplyCooldownByTag_Implementation(const FGameplayTag TagToUse)
{
	AddLooseGameplayTag(TagToUse);
}

void UAlbriteAbilitySystemComponent::Multicast_NotifyTagsRemoved_Implementation(UAbilitySystemComponent* AbilitySystemComponent, const TArray<FGameplayTag>& TagsToRemove)
{
	if (!AbilitySystemComponent) return;

	for (const FGameplayTag& Tag : TagsToRemove)
	{
		AbilitySystemComponent->RemoveLooseGameplayTag(Tag);
	}
}

