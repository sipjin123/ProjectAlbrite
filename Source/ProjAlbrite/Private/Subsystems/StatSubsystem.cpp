// Fill out your copyright notice in the Description page of Project Settings.


#include "Subsystems/StatSubsystem.h"


void UStatSubsystem::Initialize(FSubsystemCollectionBase& collection)
{
	InitEventCalledInBlueprint();
}

UWorld* UStatSubsystem::GetMyWorld()
{
	UGameInstance* GameInstance = GetGameInstance();
	return GameInstance ? GameInstance->GetWorld() : nullptr;
}

UEnemySubsystem* UStatSubsystem::GetEnemySubsystem()
{
	UGameInstance* GameInstance = GetGameInstance();
	if (GameInstance)
	{
		UWorld* WorldRef = GameInstance->GetWorld();
		if (WorldRef)
		{
			UEnemySubsystem* MyWorldSubsystem = GetWorld()->GetSubsystem<UEnemySubsystem>();
			if (MyWorldSubsystem)
			{
				return MyWorldSubsystem;
			}
		}
	}
	return nullptr;
}
