// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/MG2DCharacter.h"
#include "MGEnemy.generated.h"

class AMGSpriteVFX;

UCLASS()
class MEGAGAME2D_API AMGEnemy : public AMG2DCharacter
{
	GENERATED_BODY()

public:

	FORCEINLINE float GetContactDamage() const { return ContactDamage; }

protected:

	virtual void OnHealthDepleated() override;

	void PlayDestroyedFX();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "MG|VFX")
	TSubclassOf<AMGSpriteVFX> DestroyedVFXClass = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "MG|Damage")
	float ContactDamage = 1.f;
	
};
