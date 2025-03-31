// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/PlayerUIWidget.h"

#include "Components/ProgressBar.h"


void UPlayerUIWidget::SetHealth(float CurrHealth, float MaxHealth) const
{
	if (HealthBar) {
		HealthBar->SetPercent(CurrHealth / MaxHealth);
	}
}

void UPlayerUIWidget::SetShield(float CurrShield, float MaxShield) const
{
	if (ShieldBar) {
		ShieldBar->SetPercent(CurrShield / MaxShield);
	}
}

void UPlayerUIWidget::BindPlayerStats(AAlbritePlayerState* PlayerState)
{
	PlayerState->XPHasGained.AddDynamic(this, &UPlayerUIWidget::UpdateXPText);
	PlayerState->LevelHasGained.AddDynamic(this, &UPlayerUIWidget::UpdateLevelText);
	PlayerState->KillsHasGained.AddDynamic(this, &UPlayerUIWidget::UpdateKillsText);
}

void UPlayerUIWidget::UpdateLevelText(int32 AddedLevel)
{
	FText LevelTextNew = FText::AsNumber(AddedLevel);
	LevelText->SetText(LevelTextNew);
}

void UPlayerUIWidget::UpdateXPText(int32 AddedXP)
{
	FText XPTextNew = FText::AsNumber(AddedXP);
	XPText->SetText(XPTextNew);
}

void UPlayerUIWidget::UpdateKillsText(int32 AddedKills)
{
	FText TextNew = FText::AsNumber(AddedKills);
	KillsText->SetText(TextNew);
}

