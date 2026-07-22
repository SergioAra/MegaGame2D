// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MG2DDefinitions.h"
#include "MGProjectile.generated.h"

class UProjectileMovementComponent;
class USphereComponent;
class UPaperFlipbookComponent;
class AMGSpriteVFX;
class AMGPlayerCharacter;

UENUM(BlueprintType)
enum class ECullingType : uint8
{
	NONE		UMETA(DisplayName = "No Culling"),
	SCREEN		UMETA(DisplayName = "Screen Culling"),
	TIME		UMETA(DisplayName = "Time Culling")
};

UCLASS()
class MEGAGAME2D_API AMGProjectile : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMGProjectile();

	virtual void Destroyed() override;

	FORCEINLINE int32 GetEnergyCost() const { return EnergyCost; }
	FORCEINLINE void SetFactionType(EFactionType InFactionType) { FactionType = InFactionType; }

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "MG")
	TObjectPtr<UProjectileMovementComponent> ProjectileMovementComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "MG")
	TObjectPtr<USphereComponent> CollisionComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "MG")
	TObjectPtr<UPaperFlipbookComponent> SpriteComponent;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	UFUNCTION()
	void OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	virtual void LowTick();

	void PlayHitFX();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "MG|Projectile|VFX")
	TSubclassOf<AMGSpriteVFX> ImpactVFXClass = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "MG|Projectile")
	ECullingType CullingType = ECullingType::NONE;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "MG|Projectile", meta = (ClampMin = 0, UIMin = 0))
	int32 EnergyCost = 1;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "MG|Projectile")
	float Damage = 1;

	UPROPERTY(Transient, VisibleInstanceOnly, BlueprintReadOnly, Category = "MG|Projectile")
	TObjectPtr<AMGPlayerCharacter> OwningPlayerCharacter = nullptr;

	UPROPERTY(Transient, VisibleAnywhere, BlueprintReadOnly, Category = "MG")
	EFactionType FactionType = EFactionType::NONE;
};
