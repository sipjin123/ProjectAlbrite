// Fill out your copyright notice in the Description page of Project Settings.

#include "Utilities/AlbriteEventManager.h"

UAlbriteEventManager* UAlbriteEventManager::Instance = nullptr;
UAlbriteEventManager* UAlbriteEventManager::Get()
{
	if (!Instance)
	{
		Instance = NewObject<UAlbriteEventManager>();
		Instance->AddToRoot();
	}
	return Instance;
}