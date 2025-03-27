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
