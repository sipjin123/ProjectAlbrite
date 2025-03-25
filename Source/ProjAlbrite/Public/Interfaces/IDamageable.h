// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "IDamageable.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UIDamageable : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class PROJALBRITE_API IIDamageable
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Stats")
	float OnGetCurrentHealth();
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Stats")
	float OnGetMaxHealth();
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Stats")
	bool IsUnitDead();
};