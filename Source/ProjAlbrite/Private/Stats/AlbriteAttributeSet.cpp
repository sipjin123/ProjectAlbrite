// Fill out your copyright notice in the Description page of Project Settings.


#include "Stats/AlbriteAttributeSet.h"
#include "Net/UnrealNetwork.h"


void UAlbriteAttributeSet::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION_NOTIFY(UAlbriteAttributeSet, Health, COND_None, REPNOTIFY_OnChanged);
	DOREPLIFETIME_CONDITION_NOTIFY(UAlbriteAttributeSet, MaxHealth, COND_None, REPNOTIFY_OnChanged);
}

void UAlbriteAttributeSet::OnRep_Health(const FGameplayAttributeData& OldHealth)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAlbriteAttributeSet, Health, OldHealth);
}

void UAlbriteAttributeSet::OnRep_MaxHealth(const FGameplayAttributeData& OldHMaxealth)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAlbriteAttributeSet, MaxHealth, OldHMaxealth);
}