// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FactionType.generated.h"

UENUM(BlueprintType)
enum class EFactionType : uint8 {
	None = 0			UMETA(DisplayName = "None"),
	Heroes = 1			UMETA(DisplayName = "Heroes"),
	Enemies = 2			UMETA(DisplayName = "Enemies"),
	Neutrals = 3		UMETA(DisplayName = ";"),
};
