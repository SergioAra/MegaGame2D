// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/MG2DCharacter.h"
#include "MGEnemy.generated.h"

class AMGSpriteVFX;
class UStateTree;
class UPaperZDAnimSequence;

UCLASS()
class MEGAGAME2D_API AMGEnemy : public AMG2DCharacter
{
	GENERATED_BODY()

public:

	AMGEnemy();

	FORCEINLINE float GetContactDamage() const { return ContactDamage; }
	FORCEINLINE UStateTree* GetStateTree() const { return StateTree; }

	UFUNCTION(BlueprintCallable)
	void PlayShootingAnimation();

	// State Tree properties need to be public

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "MG|Behavior|Shooting")
	float ShootingDelay = 3.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "MG|Behavior|Shooting")
	float ShootingDelayDeviation = 1.f;

protected:

	virtual void OnHealthDepleated() override;

	void PlayDestroyedFX();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "MG|VFX")
	TSubclassOf<AMGSpriteVFX> DestroyedVFXClass = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "MG|Damage")
	float ContactDamage = 1.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "MG|AI")
	TObjectPtr<UStateTree> StateTree;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "MG|Animation")
	TObjectPtr<UPaperZDAnimSequence> ShootingAnimation;

	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "MG|Behavior|Movement")
	TObjectPtr<AActor> MovementSpline;

	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "MG|Behavior|Movement")
	bool bStartSplineForward = true;

	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "MG|Behavior|Movement")
	float SplineDurationMultiplier = 1.f;

	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "MG|Behavior|Movement")
	float SplineStartTime = 0.f;
	
};
