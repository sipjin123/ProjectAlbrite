// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/AlbritePlayerState.h"
#include "Net/UnrealNetwork.h"
#include "Utilities/AlbriteBPFunctionLibrary.h"

AAlbritePlayerState::AAlbritePlayerState()
{
}

void AAlbritePlayerState::OnRep_XPGained()
{
	XPHasGained.Broadcast(XPGained);
}

void AAlbritePlayerState::OnRep_Level()
{
	LevelHasGained.Broadcast(CurrentLevel);
}

void AAlbritePlayerState::OnRep_Kills()
{
	KillsHasGained.Broadcast(CurrentKills);
}

void AAlbritePlayerState::CheckLevel()
{
	int32 NewLevel = UAlbriteBPFunctionLibrary::UpdateLevelByXP(XPGained);
	if (HasAuthority() && NewLevel > CurrentLevel)
	{
		CurrentLevel = NewLevel;
	}
}

void AAlbritePlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME_CONDITION_NOTIFY(AAlbritePlayerState, CurrentLevel, COND_None, REPNOTIFY_OnChanged);
	DOREPLIFETIME_CONDITION_NOTIFY(AAlbritePlayerState, XPGained, COND_None, REPNOTIFY_OnChanged);
	DOREPLIFETIME_CONDITION_NOTIFY(AAlbritePlayerState, CurrentKills, COND_None, REPNOTIFY_OnChanged);
}
