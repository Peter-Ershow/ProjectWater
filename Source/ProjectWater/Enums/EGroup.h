// YourEnum.h

#pragma once

#include "CoreMinimal.h"
#include "UObject/ObjectMacros.h"
#include "EGroup.generated.h"

UENUM(BlueprintType)
enum class EGroup : uint8 {
	Visual       UMETA(DisplayName="Visual"),
	Auditory        UMETA(DisplayName="Auditory"),
	Haptics        UMETA(DisplayName="Haptics"),
	Control        UMETA(DisplayName="Control"),
	Combined        UMETA(DisplayName="Combined"),
};