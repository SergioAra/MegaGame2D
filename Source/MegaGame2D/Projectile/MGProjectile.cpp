// Fill out your copyright notice in the Description page of Project Settings.


#include "Projectile/MGProjectile.h"
#include "VFX/MGSpriteVFX.h"

#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/SphereComponent.h"
#include "PaperFlipbookComponent.h"

static FName SpriteCollisionProfileName(TEXT("NoCollision"));
static FName CollisionProfileName(TEXT("Projectile"));

// Sets default values
AMGProjectile::AMGProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovementComponent"));
	ProjectileMovementComponent->ProjectileGravityScale = 0.f;
	ProjectileMovementComponent->InitialSpeed = 1000.f;
	ProjectileMovementComponent->MaxSpeed = 1000.f;

	//ProjectileMovementComponent->bShouldBounce = true;

	CollisionComponent = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionComponent"));
	CollisionComponent->SetCollisionProfileName(CollisionProfileName);
	SetRootComponent(CollisionComponent);

	SpriteComponent = CreateDefaultSubobject<UPaperFlipbookComponent>(TEXT("SpriteComponent"));
	SpriteComponent->SetupAttachment(RootComponent);
	SpriteComponent->SetGenerateOverlapEvents(false);
	SpriteComponent->SetCollisionProfileName(SpriteCollisionProfileName);

}

// Called when the game starts or when spawned
void AMGProjectile::BeginPlay()
{
	Super::BeginPlay();

	if (CollisionComponent)
	{
		CollisionComponent->OnComponentHit.AddDynamic(this, &AMGProjectile::OnHit);
	}
	
}

void AMGProjectile::Destroyed()
{
	if (ImpactVFXClass)
	{
		FActorSpawnParameters SpawnParams;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		GetWorld()->SpawnActor<AMGSpriteVFX>(ImpactVFXClass, GetActorLocation(), GetActorRotation(), SpawnParams);
	}

	Super::Destroyed();
}

void AMGProjectile::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	Destroy();
}