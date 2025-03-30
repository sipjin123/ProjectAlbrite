// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/AlbritePlayerState.h"
#include "Net/UnrealNetwork.h"

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

void AAlbritePlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME_CONDITION_NOTIFY(AAlbritePlayerState, CurrentLevel, COND_None, REPNOTIFY_OnChanged);
	DOREPLIFETIME_CONDITION_NOTIFY(AAlbritePlayerState, XPGained, COND_None, REPNOTIFY_OnChanged);
}
