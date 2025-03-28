// Fill out your copyright notice in the Description page of Project Settings.


#include "Stats/AlbriteAttributeSet.h"
#include "Net/UnrealNetwork.h"


UAlbriteAttributeSet::UAlbriteAttributeSet()
{
	// Assign default values directly
	// TODO: Create a data table and process contents from there
	Health.SetBaseValue(100.0f);
	Health.SetCurrentValue(100.0f);
	MaxHealth.SetBaseValue(100.0f);
	MaxHealth.SetCurrentValue(100.0f);

	Shield.SetBaseValue(0.0f);
	Shield.SetCurrentValue(0.0f);
}

void UAlbriteAttributeSet::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION_NOTIFY(UAlbriteAttributeSet, Health, COND_None, REPNOTIFY_OnChanged);
	DOREPLIFETIME_CONDITION_NOTIFY(UAlbriteAttributeSet, MaxHealth, COND_None, REPNOTIFY_OnChanged);
	DOREPLIFETIME_CONDITION_NOTIFY(UAlbriteAttributeSet, Shield, COND_None, REPNOTIFY_OnChanged);
	DOREPLIFETIME_CONDITION_NOTIFY(UAlbriteAttributeSet, DamageReceived, COND_None, REPNOTIFY_OnChanged);
}

void UAlbriteAttributeSet::OnRep_Health(const FGameplayAttributeData& OldHealth)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAlbriteAttributeSet, Health, OldHealth);
}

void UAlbriteAttributeSet::OnRep_MaxHealth(const FGameplayAttributeData& OldHMaxealth)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAlbriteAttributeSet, MaxHealth, OldHMaxealth);
}

void UAlbriteAttributeSet::OnRep_Shield(const FGameplayAttributeData& OldShield)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAlbriteAttributeSet, Shield, OldShield);
}

void UAlbriteAttributeSet::OnRep_DamageReceived(const FGameplayAttributeData& OldDamageReceived)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAlbriteAttributeSet, DamageReceived, OldDamageReceived);
}
