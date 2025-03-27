// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AlbriteEventManager.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "AlbriteBPFunctionLibrary.generated.h"

/**
 * The helper class that allows collecting universally used repetitive functions
 * Author: Burlin
 */
UCLASS(Blueprintable) 
class PROJALBRITE_API UAlbriteBPFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_UCLASS_BODY()
	
	//FORCEINLNE function
	static FORCEINLINE bool IsValid(AActor* TheActor)
	{
		if(!TheActor) return false;
		if(!TheActor->IsValidLowLevel()) return false;
		return true;
	}
public:
	// Retrieves all actors within a radius
	UFUNCTION(BlueprintCallable, meta = (WorldContext = WorldContextObject))
	static TArray<AActor*> GetActorsWithinRadius(FVector Origin, float Radius, UObject * WorldContextObject);
	
	UFUNCTION(BlueprintCallable, Category = "Events")
	static void OnSpawnWorldText(FVector Location, EDebugColor ColorType, FString Message)
	{
		UAlbriteEventManager::Get()->OnSpawnWorldText.Broadcast(Message, ColorType, Location);
	}
};
