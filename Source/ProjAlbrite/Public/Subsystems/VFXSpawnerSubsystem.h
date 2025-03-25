// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "VFXSpawnerSubsystem.generated.h"

/**
 * 
 */
UCLASS(Abstract, Blueprintable)
class PROJALBRITE_API UVFXSpawnerSubsystem : public UWorldSubsystem
{
	GENERATED_BODY()

public:
	
	UFUNCTION(BlueprintImplementableEvent)
	void InitEventCalledInBlueprint();
	
protected:
	virtual void Initialize(FSubsystemCollectionBase& collection) override;
};
