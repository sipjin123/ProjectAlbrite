// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameEnums.generated.h"

UENUM(BlueprintType)
enum class ECharacterType : uint8 {
	None = 0			UMETA(DisplayName = "None"),
	Vanguard = 1		UMETA(DisplayName = "Default"),
	Rogue = 2			UMETA(DisplayName = "Rogue"),
	Alchemist = 3		UMETA(DisplayName = "Alchemist"),
	Engineer = 4		UMETA(DisplayName = "Engineer"),
};
