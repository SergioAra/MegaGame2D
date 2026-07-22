// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/MG2DCharacter.h"
#include "Component/MGHealthComponent.h"

#include "PaperFlipbookComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/CapsuleComponent.h"

static FName CollisionProfileName(TEXT("2DCharacter"));

AMG2DCharacter::AMG2DCharacter()
{
	PrimaryActorTick.bCanEverTick = false;

	HealthComponent = CreateDefaultSubobject<UMGHealthComponent>(TEXT("HealthComponent"));

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

	UCapsuleComponent* CapsuleComp = GetCapsuleComponent();
	if (CapsuleComp)
	{
		CapsuleComp->SetCollisionProfileName(CollisionProfileName);
	}
}

void AMG2DCharacter::BeginPlay()
{
	Super::BeginPlay();
	if (HealthComponent)
	{
		HealthComponent->OnHealthDepleated.AddDynamic(this, &AMG2DCharacter::OnHealthDepleated);
		HealthComponent->OnDamageTaken.AddDynamic(this, &AMG2DCharacter::OnDamageTaken);
	}

	UPaperFlipbookComponent* SpriteComponent = GetSprite();
	if (SpriteComponent)
	{
		DynamicMaterialInstance = SpriteComponent->CreateDynamicMaterialInstance(0);
	}
}

void AMG2DCharacter::OnHealthDepleated()
{
	Destroy();
}

void AMG2DCharacter::OnDamageTaken(AActor* DamageSource)
{
	OnDamageVFX();
}
