// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PaperZDCharacter.h"
#include "MG2DDefinitions.h"
#include "MG2DCharacter.generated.h"

class UMGHealthComponent;


UCLASS()
class MEGAGAME2D_API AMG2DCharacter : public APaperZDCharacter
{
	GENERATED_BODY()

public:

	AMG2DCharacter();

	FORCEINLINE EFactionType GetFactionType() const { return FactionType; }

protected:

	virtual void BeginPlay() override;

	UFUNCTION()
	virtual void OnHealthDepleated();

	UFUNCTION()
	virtual void OnDamageTaken(AActor* DamageSource);

	UFUNCTION(BlueprintImplementableEvent)
	void OnDamageVFX();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "MG")
	TObjectPtr<UMGHealthComponent> HealthComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "MG")
	EFactionType FactionType = EFactionType::NONE;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "MG|VFX")
	float DamageVFXPlayRate = 1.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "MG|VFX")
	float DamageVFXScale = 1.0f;

	UPROPERTY(Transient, VisibleInstanceOnly, BlueprintReadOnly, Category = "MG")
	TObjectPtr<UMaterialInstanceDynamic> DynamicMaterialInstance;
	
};
