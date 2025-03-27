// Fill out your copyright notice in the Description page of Project Settings.


#include "CombatUnitWidget.h"
#include "Components/ProgressBar.h"


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
