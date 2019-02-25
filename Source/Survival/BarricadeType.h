#pragma once

#include "CoreMinimal.h"
#include "BarricadeType.generated.h"

UENUM(BlueprintType)
enum class EBarricadeType : uint8 {
	EDumpster = 0			UMETA(DisplayName = "Dumpster")
};