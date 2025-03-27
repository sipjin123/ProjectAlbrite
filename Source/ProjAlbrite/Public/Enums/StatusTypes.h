#pragma once

#include "CoreMinimal.h"
#include "StatusTypes.generated.h"

UENUM(BlueprintType)
enum class EStatusType : uint8 {
	None = 0			UMETA(DisplayName = "None"),
	Stunned = 1			UMETA(DisplayName = "Stunned"),
	Vulnerable = 2		UMETA(DisplayName = "Vulnerable"),
	Weakened = 3		UMETA(DisplayName = "Weakened"),
	Invulnerable = 4	UMETA(DisplayName = "Invulnerable"),
};
