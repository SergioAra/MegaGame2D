// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/MGEnemy.h"
#include "VFX/MGSpriteVFX.h"

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
