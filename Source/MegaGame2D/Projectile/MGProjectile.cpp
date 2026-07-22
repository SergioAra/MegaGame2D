// Fill out your copyright notice in the Description page of Project Settings.


#include "Projectile/MGProjectile.h"
#include "VFX/MGSpriteVFX.h"
#include "Character/MGPlayerCharacter.h"

#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/SphereComponent.h"
#include "PaperFlipbookComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Blueprint/WidgetLayoutLibrary.h"

static FName SpriteCollisionProfileName(TEXT("NoCollision"));
static FName CollisionProfileName(TEXT("Projectile"));
static float LowDeltaTime(0.1f);

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
		CollisionComponent->OnComponentBeginOverlap.AddDynamic(this, &AMGProjectile::OnBeginOverlap);	
	}

	AActor* OwningActor = GetOwner();

	AMG2DCharacter* OwningCharacter = Cast<AMG2DCharacter>(OwningActor);
	if (OwningCharacter)
	{
		FactionType = OwningCharacter->GetFactionType();
		OwningPlayerCharacter = Cast<AMGPlayerCharacter>(OwningActor);
	}

	FTimerHandle LowTickTimerHandle;
	GetWorldTimerManager().SetTimer(LowTickTimerHandle, this, &AMGProjectile::LowTick, LowDeltaTime, true);
	
}

void AMGProjectile::Destroyed()
{
	Super::Destroyed();

	if (OwningPlayerCharacter)
	{
		OwningPlayerCharacter->RestoreEnergy(EnergyCost);
	}
}

void AMGProjectile::PlayHitFX()
{
	if (ImpactVFXClass)
	{
		FActorSpawnParameters SpawnParams;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		GetWorld()->SpawnActor<AMGSpriteVFX>(ImpactVFXClass, GetActorLocation(), GetActorRotation(), SpawnParams);
	}
}

void AMGProjectile::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	PlayHitFX();
	Destroy();
}

void AMGProjectile::OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	AMG2DCharacter* OverlappedCharacter = Cast<AMG2DCharacter>(OtherActor);

	if (OverlappedCharacter && OverlappedCharacter != GetOwner())
	{
		EFactionType OverlappedFaction = OverlappedCharacter->GetFactionType();

		if (OverlappedFaction == EFactionType::NONE || OverlappedFaction != FactionType)
		{
			PlayHitFX();
			Destroy();
			UGameplayStatics::ApplyDamage(OverlappedCharacter, Damage, GetInstigatorController(), this, nullptr);
		}
	}
}

void AMGProjectile::LowTick()
{
	switch (CullingType)
	{
	case ECullingType::SCREEN:
		{
			APlayerController* PC = UGameplayStatics::GetPlayerController(this, 0);
			FVector2D OutLocation;
			UGameplayStatics::ProjectWorldToScreen(PC, GetActorLocation(), OutLocation);
			FVector2D ViewportSize = UWidgetLayoutLibrary::GetViewportSize(this);

			if (OutLocation.X < 0.f || OutLocation.X > ViewportSize.X || OutLocation.Y < 0.f || OutLocation.Y > ViewportSize.Y)
			{
				Destroy();
			}
		}
		break;

	case ECullingType::TIME:
		break;
	}
}
