// Fill out your copyright notice in the Description page of Project Settings.


#include "CombatUnitWidget.h"

#include "Components/Image.h"
#include "Components/PanelWidget.h"
#include "Components/ProgressBar.h"
#include "Components/SizeBox.h"


void UCombatUnitWidget::SetHealth(float CurrHealth, float MaxHealth) const
{
	if (HealthBar) {
		HealthBar->SetPercent(CurrHealth / MaxHealth);
	}
}

void UCombatUnitWidget::SetShield(float CurrShield, float MaxShield) const
{
	if (ShieldBar) {
		ShieldBar->SetPercent(CurrShield / MaxShield);
	}
}

void UCombatUnitWidget::RegisterStatusBinding(UAlbriteAbilitySystemComponent* AbilityComponent)
{
	ECombatElementType Fire = ECombatElementType::Fire;
	AbilityComponent->RegisterGameplayTagEvent(
		FGameplayTag::RequestGameplayTag(FName("Element.Fire")),
		EGameplayTagEventType::NewOrRemoved)
	.AddLambda([this, Fire](const FGameplayTag Tag, int32 NewCount)
	{
		OnStatusChanged(Fire, NewCount);
	});

	ECombatElementType Ice = ECombatElementType::Ice;
	AbilityComponent->RegisterGameplayTagEvent(
		FGameplayTag::RequestGameplayTag(FName("Element.Ice")),
		EGameplayTagEventType::NewOrRemoved)
	.AddLambda([this, Ice](const FGameplayTag Tag, int32 NewCount)
	{
		OnStatusChanged(Ice, NewCount);
	});
		
	ECombatElementType Lightning = ECombatElementType::Lightning;
	AbilityComponent->RegisterGameplayTagEvent(
		FGameplayTag::RequestGameplayTag(FName("Element.Lightning")),
		EGameplayTagEventType::NewOrRemoved)
	.AddLambda([this, Lightning](const FGameplayTag Tag, int32 NewCount)
	{
		OnStatusChanged(Lightning, NewCount);
	});
}

void UCombatUnitWidget::OnStatusChanged(ECombatElementType ElementType, int32 NewCount)
{
	FString ElementString = UEnum::GetDisplayValueAsText(ElementType).ToString();

	switch (ElementType)
	{
	case ECombatElementType::Fire:
		{
			FireElementStatus->SetVisibility(NewCount == 0 ? ESlateVisibility::Collapsed : ESlateVisibility::Visible);
		}
		break;
	case ECombatElementType::Ice:
		{
			IceElementStatus->SetVisibility(NewCount == 0 ? ESlateVisibility::Collapsed : ESlateVisibility::Visible);
		}
		break;
	case ECombatElementType::Lightning:
		{
			LightningElementStatus->SetVisibility(NewCount == 0 ? ESlateVisibility::Collapsed : ESlateVisibility::Visible);
		}
		break;
	}
}
