#pragma once

#include "CoreMinimal.h"
#include "BuffTypes.generated.h"

UENUM(BlueprintType)
enum class EBuffType : uint8 {
	None = 0			UMETA(DisplayName = "None"),
	SpeedBoost = 1		UMETA(DisplayName = "SpeedBoost"),
	DefenseBoost = 2	UMETA(DisplayName = "DefenseBoost"),
	AttackBoost = 3		UMETA(DisplayName = "AttackBoost"),
};
