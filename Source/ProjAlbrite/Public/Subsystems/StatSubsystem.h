// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "StatSubsystem.generated.h"

/**
 * 
 */

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FKilledTarget, AActor*, Target, AActor*, Killer);

UCLASS(Abstract, Blueprintable)
class PROJALBRITE_API UStatSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	
	UFUNCTION(BlueprintImplementableEvent)
	void InitEventCalledInBlueprint();
	
protected:
	virtual void Initialize(FSubsystemCollectionBase& collection) override;

public:
	UPROPERTY(BlueprintCallable, BlueprintAssignable)
	FKilledTarget KilledTarget;
};
