// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "GameFramework/PlayerState.h"
#include "AlbritePlayerState.generated.h"

/**
 * 
 */

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FXPHasGained, int32, AddedXP);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FLevelHasGained, int32, AddedLevel);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FGainedKills, int32, AddedXP);
UCLASS()
class PROJALBRITE_API AAlbritePlayerState : public APlayerState
{
	GENERATED_BODY()
public:
	// TODO: Put this in a data table
	static const int XPPerKill = 10;
	
	AAlbritePlayerState();
	
	UPROPERTY(Replicated, BlueprintReadWrite, ReplicatedUsing = OnRep_XPGained)
	int32 XPGained = 0;
	UFUNCTION()
	void OnRep_XPGained();

	UPROPERTY(BlueprintAssignable, BlueprintCallable)
	FXPHasGained XPHasGained;
	
	UPROPERTY(Replicated, BlueprintReadWrite, ReplicatedUsing = OnRep_Level)
	int32 CurrentLevel = 1;
	UFUNCTION()
	void OnRep_Level();
	
	UPROPERTY(BlueprintAssignable, BlueprintCallable)
	FLevelHasGained LevelHasGained;

	// Calculates if level changes
	UFUNCTION(BlueprintCallable)
	void CheckLevel();

	
	UPROPERTY(Replicated, BlueprintReadWrite, ReplicatedUsing = OnRep_Kills)
	int32 CurrentKills = 0;
	UFUNCTION()
	void OnRep_Kills();
	UPROPERTY(BlueprintAssignable, BlueprintCallable)
	FGainedKills KillsHasGained;
};
