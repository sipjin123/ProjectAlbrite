// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CombatUnitWidget.generated.h"

/**
 * 
 */
UCLASS()
class PROJALBRITE_API UCombatUnitWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintCallable)
	void SetHealth(float CurrHealth, float MaxHealth) const;
	UFUNCTION(BlueprintCallable)
	void SetShield(float CurrShield, float MaxShield) const;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UProgressBar* HealthBar;
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UProgressBar* ShieldBar;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UHorizontalBox* StatusContainer;
};