// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "EnemySubsystem.generated.h"

/**
 * 
 */

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FSpawnEnemy, int, EnemyCount);
UCLASS(Abstract, Blueprintable)
class PROJALBRITE_API UEnemySubsystem : public UWorldSubsystem
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintImplementableEvent)
	void InitEventCalledInBlueprint();
	
protected:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	
public:
	/** Track enemies in the world (starts with 5) **/
	UPROPERTY(BlueprintReadWrite)
	int EnemiesActive = 5;

	UPROPERTY(BlueprintCallable, BlueprintAssignable)
	FSpawnEnemy SpawnEnemy;
};
