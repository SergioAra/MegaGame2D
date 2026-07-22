// Fill out your copyright notice in the Description page of Project Settings.


#include "Component/MGHealthComponent.h"

// Sets default values for this component's properties
UMGHealthComponent::UMGHealthComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

}


// Called when the game starts
void UMGHealthComponent::BeginPlay()
{
	Super::BeginPlay();

	CurrentHealth = MaxHealth;

	AActor* Owner = GetOwner();

	if (Owner)
	{
		Owner->OnTakeAnyDamage.AddDynamic(this, &UMGHealthComponent::OnTakeAnyDamage);
	}
	
}

void UMGHealthComponent::OnTakeAnyDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType, AController* InstigatedBy, AActor* DamageCauser)
{
	if (bIsDead)
	{
		return;
	}

	if (bIsInvincible)
	{
		return;
	}

	if (CurrentHealth > 0.f)
	{
		CurrentHealth = FMath::Max(CurrentHealth - Damage, 0.f);
		OnDamageTaken.Broadcast(DamageCauser);
		if (CurrentHealth <= 0.f)
		{
			bIsDead = true;
			OnHealthDepleated.Broadcast();
		}
	}
}

