// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "MGHealthComponent.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE(FHealthDepleatedSignature);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FDamageTakenSignature, AActor*, DamageSource);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class MEGAGAME2D_API UMGHealthComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UMGHealthComponent();

	FORCEINLINE bool IsInvincible() const { return bIsInvincible; }
	FORCEINLINE void SetIsInvincible(bool bInIsInvincible) { bIsInvincible = bInIsInvincible; }	

	void KillOwner();

	UPROPERTY(BlueprintAssignable, Category = "MG")
	FHealthDepleatedSignature OnHealthDepleated;

	UPROPERTY(BlueprintAssignable, Category = "MG")
	FDamageTakenSignature OnDamageTaken;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	UFUNCTION()
	void OnTakeAnyDamage(AActor* DamagedActor, float Damage, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser);

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "MG")
	float MaxHealth = 10.f;

	UPROPERTY(Transient, VisibleInstanceOnly, BlueprintReadOnly, Category = "MG")
	float CurrentHealth = 0.f;

	UPROPERTY(Transient, VisibleInstanceOnly, BlueprintReadOnly, Category = "MG")
	bool bIsDead = false;

	UPROPERTY(Transient, VisibleInstanceOnly, BlueprintReadOnly, Category = "MG")
	bool bIsInvincible = false;

};
