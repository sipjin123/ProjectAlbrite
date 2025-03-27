// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffectExecutionCalculation.h"
#include "ExCal_ShieldDeplete.generated.h"

/**
 * 
 */
UCLASS()
class PROJALBRITE_API UExCal_ShieldDeplete : public UGameplayEffectExecutionCalculation
{
	GENERATED_BODY()
	
	UExCal_ShieldDeplete();
	
	virtual void Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams,
		FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const override;
};
