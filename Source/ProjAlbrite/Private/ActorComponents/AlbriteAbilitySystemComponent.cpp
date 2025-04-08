// Fill out your copyright notice in the Description page of Project Settings.


#include "ActorComponents/AlbriteAbilitySystemComponent.h"

#include "Abilities/AlbriteBaseGameplayAbility.h"
#include "Interfaces/IDamageable.h"

void UAlbriteAbilitySystemComponent::NotifyAbilityActivated(const FGameplayAbilitySpecHandle Handle, UGameplayAbility* Ability)
{
	//bool NewVal = HasMatchingGameplayTag(FGameplayTag::RequestGameplayTag(FName("Status.Invulnerable")));

	UAlbriteBaseGameplayAbility* BaseAbility = Cast<UAlbriteBaseGameplayAbility>(Ability);
	bool NewVal = HasMatchingGameplayTag(FGameplayTag::RequestGameplayTag(FName("Ability.Active")));
	if (GetOwnerRole() == ROLE_Authority && BaseAbility && BaseAbility->bDisabledMovement)
	{
		if (NewVal && GetOwner()->GetClass()->ImplementsInterface(UIDamageable::StaticClass()))
		{
			IIDamageable::Execute_SetCastAbility(GetOwner(), true);
		}
		// Do logic here for when invulnerability ends
		// UE_LOG(LogTemp, Warning, TEXT("An Ability has Ended! %d"), NewVal ? 1 : 0);
	}
	
	Super::NotifyAbilityActivated(Handle, Ability);
}

void UAlbriteAbilitySystemComponent::NotifyAbilityEnded(FGameplayAbilitySpecHandle Handle, UGameplayAbility* Ability, bool bWasCancelled)
{
	UAlbriteBaseGameplayAbility* BaseAbility = Cast<UAlbriteBaseGameplayAbility>(Ability);
	if (BaseAbility && BaseAbility->bDisabledMovement
		&& GetOwnerRole() == ROLE_Authority
		&& GetOwner()->GetClass()->ImplementsInterface(UIDamageable::StaticClass()))
	{
		IIDamageable::Execute_SetCastAbility(GetOwner(), false);
	}
	Super::NotifyAbilityEnded(Handle, Ability, bWasCancelled);
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

