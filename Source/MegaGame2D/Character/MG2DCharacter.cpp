// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/MG2DCharacter.h"
#include "Component/MGHealthComponent.h"
#include "Projectile/MGProjectile.h"

#include "PaperFlipbookComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

static FName MuzzleSocketName(TEXT("Muzzle"));

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
}

void AMG2DCharacter::SpawnProjectile(TSubclassOf<AMGProjectile>& ProjectileClassToSpawn)
{
	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	SpawnParams.Owner = this;
	SpawnParams.Instigator = this;

	FVector ShootLocation = GetActorLocation();
	UPaperFlipbookComponent* SpriteComponent = GetSprite();
	if (SpriteComponent && SpriteComponent->DoesSocketExist(MuzzleSocketName))
	{
		ShootLocation = SpriteComponent->GetSocketLocation(MuzzleSocketName);
	}
	else
	{
		// Try to offset the shoot location to the front of the character if no socket is found. TODO: delay the spawn until the animation is ready
		ShootLocation += GetActorForwardVector() * 60.f + GetActorUpVector() * -4.5f;
	}

	//DrawDebugSphere(GetWorld(), ShootLocation, 5.f, 12, FColor::Red, false, 1.f);
	const FVector& ProjectileDirection = GetShootingDirection();
	GetWorld()->SpawnActor<AMGProjectile>(ProjectileClassToSpawn, ShootLocation, ProjectileDirection.Rotation(), SpawnParams);
}

void AMG2DCharacter::ShootProjectile()
{
	SpawnProjectile(ProjectileClass);
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

FVector AMG2DCharacter::GetShootingDirection() const
{
	return GetActorForwardVector();
}
