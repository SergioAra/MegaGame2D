#pragma once

#include "CoreMinimal.h"

#define ECC_Projectile ECC_GameTraceChannel1
#define ECC_Enemy ECC_GameTraceChannel2
#define ECC_Volume ECC_GameTraceChannel3
#define ECC_Player ECC_GameTraceChannel4


UENUM(BlueprintType)
enum class EFactionType : uint8
{
	NONE		UMETA(DisplayName = "No Faction"), // Damage Everyone and Damaged by everyone
	PLAYER		UMETA(DisplayName = "Player"),
	Enemy		UMETA(DisplayName = "Enemy")
};