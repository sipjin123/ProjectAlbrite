// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "AlbriteEventManager.generated.h"

/**
 * A helper class for custom game events
 * Author: Burlin
 */
UCLASS(Blueprintable)
class PROJALBRITE_API UAlbriteEventManager : public UObject
{
public:
	GENERATED_BODY()

	// Singleton accessor
	UFUNCTION(BlueprintCallable)
	static UAlbriteEventManager* Get();
	
private:
	// Internal instance
	static UAlbriteEventManager* Instance;
};