// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/MG2DCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"

AMG2DCharacter::AMG2DCharacter()
{
	PrimaryActorTick.bCanEverTick = false;

	UCharacterMovementComponent* MovementComponent = GetCharacterMovement();

	if (MovementComponent)
	{
		MovementComponent->SetPlaneConstraintEnabled(true);
		MovementComponent->SetPlaneConstraintNormal(FVector(0.f, 1.f, 0.f));

		MovementComponent->bUseFlatBaseForFloorChecks = true;

		MovementComponent->GravityScale = 5.5f;
		MovementComponent->JumpZVelocity = 600.f;
		MovementComponent->AirControl = 0.8f;
	}
}
