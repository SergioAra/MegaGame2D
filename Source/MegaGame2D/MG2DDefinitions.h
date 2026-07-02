#pragma once

#include "CoreMinimal.h"


UENUM(BlueprintType)
enum class EFactionType : uint8
{
	NONE		UMETA(DisplayName = "No Faction"), // Damage Everyone and Damaged by everyone
	PLAYER		UMETA(DisplayName = "Player"),
	Enemy		UMETA(DisplayName = "Enemy")
};