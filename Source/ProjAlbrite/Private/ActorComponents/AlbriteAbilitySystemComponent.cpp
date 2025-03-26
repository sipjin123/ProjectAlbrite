// Fill out your copyright notice in the Description page of Project Settings.


#include "ActorComponents/AlbriteAbilitySystemComponent.h"

void UAlbriteAbilitySystemComponent::NotifyAbilityActivated(const FGameplayAbilitySpecHandle Handle,
	UGameplayAbility* Ability)
{
	Super::NotifyAbilityActivated(Handle, Ability);
}
