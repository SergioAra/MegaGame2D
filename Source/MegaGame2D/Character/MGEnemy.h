// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/MG2DCharacter.h"
#include "MGEnemy.generated.h"

class AMGSpriteVFX;
class UStateTree;
class UPaperZDAnimSequence;
class USoundCue;

UCLASS()
class MEGAGAME2D_API AMGEnemy : public AMG2DCharacter
{
	GENERATED_BODY()

public:

	AMGEnemy();

	FORCEINLINE float GetContactDamage() const { return ContactDamage; }
	FORCEINLINE UStateTree* GetStateTree() const { return StateTree; }

	// State Tree properties need to be public

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "MG|Behavior|Shooting")
	float ShootingDelay = 3.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "MG|Behavior|Shooting")
	float ShootingDelayDeviation = 1.f;

protected:

	virtual void OnHealthDepleated() override;

	void PlayDestroyedFX();

	UFUNCTION(BlueprintCallable)
	void PlayShootingAnimation();

	UFUNCTION(BlueprintCallable)
	void PlayerDetected();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "MG|VFX")
	TSubclassOf<AMGSpriteVFX> DestroyedVFXClass = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "MG|Damage")
	float ContactDamage = 1.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "MG|AI")
	TObjectPtr<UStateTree> StateTree;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "MG|Animation")
	TObjectPtr<UPaperZDAnimSequence> ShootingAnimation;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "MG|Audio")
	TObjectPtr<USoundCue> DefeatedSound;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "MG|Audio")
	TObjectPtr<USoundCue> PlayerDetectedSound;

	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "MG|Behavior|Movement")
	TObjectPtr<AActor> MovementSpline;

	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "MG|Behavior|Movement")
	bool bStartSplineForward = true;

	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "MG|Behavior|Movement")
	float SplineDurationMultiplier = 1.f;

	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "MG|Behavior|Movement")
	float SplineStartTime = 0.f;
	
};
