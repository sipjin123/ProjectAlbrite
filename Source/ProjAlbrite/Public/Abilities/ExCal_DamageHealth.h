// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffectExecutionCalculation.h"
#include "ExCal_DamageHealth.generated.h"

/**
 * 
 */
UCLASS()
class PROJALBRITE_API UExCal_DamageHealth : public UGameplayEffectExecutionCalculation
{
	GENERATED_BODY()
	
public:
	UExCal_DamageHealth();
	
	virtual void Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams,
		FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const override;
};
