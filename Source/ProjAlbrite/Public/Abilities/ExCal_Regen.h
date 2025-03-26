// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffectExecutionCalculation.h"
#include "ExCal_Regen.generated.h"

/**
 * 
 */
UCLASS()
class PROJALBRITE_API UExCal_Regen : public UGameplayEffectExecutionCalculation
{
	GENERATED_BODY()

	UExCal_Regen();
	
	virtual void Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams,
		FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const override;
};
