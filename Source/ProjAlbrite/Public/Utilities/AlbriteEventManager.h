// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "AlbriteEventManager.generated.h"

/**
 * A helper class for custom game events
 * Author: Burlin
 */

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnSpawnWorldText, FString, Message, EDebugColor, ColorType, FVector, Location);
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

public:
	// A delegate to be binded using VFX Spawner subsystem in blueprints
	UPROPERTY(BlueprintAssignable)
	FOnSpawnWorldText OnSpawnWorldText;
};