// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/MGEnemy.h"
#include "VFX/MGSpriteVFX.h"

#include "Components/CapsuleComponent.h"

static FName EnemyCollisionProfileName(TEXT("2DEnemy"));

AMGEnemy::AMGEnemy()
{
	UCapsuleComponent* CapsuleComp = GetCapsuleComponent();
	if (CapsuleComp)
	{
		CapsuleComp->SetCollisionProfileName(EnemyCollisionProfileName);
	}
}

void AMGEnemy::OnHealthDepleated()
{
	Super::OnHealthDepleated();
	PlayDestroyedFX();
}

void AMGEnemy::PlayDestroyedFX()
{
	if (DestroyedVFXClass)
	{
		FActorSpawnParameters SpawnParams;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		GetWorld()->SpawnActor<AMGSpriteVFX>(DestroyedVFXClass, GetActorLocation(), GetActorRotation(), SpawnParams);
	}
}
