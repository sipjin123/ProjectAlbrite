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
UCLASS()
class PROJALBRITE_API AAlbritePlayerState : public APlayerState
{
	GENERATED_BODY()
public:

	AAlbritePlayerState();
	
	UPROPERTY(Replicated, BlueprintReadWrite, ReplicatedUsing = OnRep_XPGained)
	int32 XPGained;
	UFUNCTION()
	void OnRep_XPGained();

	UPROPERTY(BlueprintAssignable, BlueprintCallable)
	FXPHasGained XPHasGained;
	
	UPROPERTY(Replicated, BlueprintReadWrite, ReplicatedUsing = OnRep_Level)
	int32 CurrentLevel;
	UFUNCTION()
	void OnRep_Level();
	
	UPROPERTY(BlueprintAssignable, BlueprintCallable)
	FLevelHasGained LevelHasGained;
};
