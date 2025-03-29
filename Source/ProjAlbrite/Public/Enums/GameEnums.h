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

UENUM(BlueprintType)
enum class EAbilityInputID : uint8 {
	None = 0		UMETA(DisplayName = "None"),
	Attack = 1		UMETA(DisplayName = "Attack"),
	Dash = 2		UMETA(DisplayName = "Dash"),
	Defense = 3		UMETA(DisplayName = "Defense"),
	Special = 4		UMETA(DisplayName = "Special"),
	Ultimate = 5	UMETA(DisplayName = "Ultimate"),
	Aim = 6	UMETA(DisplayName = "Aim"),
};

UENUM(BlueprintType)
enum class EDebugColor : uint8 {
	None = 0,
	Blue = 1,
	Red = 2,
	Green = 3,
	Yellow = 4,
	Black = 5,
	BlazeFrost = 6,
	FrostShock = 7,
	LightningInferno = 8
};

UENUM(BlueprintType)
enum class ECombatElementType : uint8 {
	None = 0 UMETA(DisplayName = "None"),
	Fire = 1 UMETA(DisplayName = "Fire"),
	Lightning = 2 UMETA(DisplayName = "Lightning"),
	Water = 3 UMETA(DisplayName = "Water"),
	Wind = 4 UMETA(DisplayName = "Wind"),
	Acid = 5 UMETA(DisplayName = "Acid"),
	Dark = 6 UMETA(DisplayName = "Dark"),
	Light = 7 UMETA(DisplayName = "Light"),
	Ice = 8 UMETA(DisplayName = "Ice"),
};