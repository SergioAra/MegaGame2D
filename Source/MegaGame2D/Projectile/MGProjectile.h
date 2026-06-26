// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MGProjectile.generated.h"

class UProjectileMovementComponent;
class USphereComponent;
class UPaperFlipbookComponent;
class AMGSpriteVFX;

UCLASS()
class MEGAGAME2D_API AMGProjectile : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMGProjectile();

	virtual void Destroyed() override;

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

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "MG")
	TSubclassOf<AMGSpriteVFX> ImpactVFXClass = nullptr;

};
