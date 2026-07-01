// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/MG2DCharacter.h"
#include "MGPlayerCharacter.generated.h"

class UCameraComponent;
class USpringArmComponent;
class AMGProjectile;

UCLASS()
class MEGAGAME2D_API AMGPlayerCharacter : public AMG2DCharacter
{
	GENERATED_BODY()

public:

	AMGPlayerCharacter();

	UFUNCTION(BlueprintCallable)
	FORCEINLINE bool IsShooting() const { return bIsShooting; }
	FORCEINLINE bool SetIsShooting(bool bInIsShooting) { return bIsShooting = bInIsShooting; }

	void InputMove(const FVector2D& AxisValue);
	void StartInputJump();
	void CancelInputJump();
	void InputShoot();

protected:

	UFUNCTION()
	void OnShootingStateFinished();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "MG")
	TObjectPtr<USpringArmComponent> SpringArmComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "MG")
	TObjectPtr<UCameraComponent> CameraComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "MG|Shooting|Projectile")
	TSubclassOf<AMGProjectile> ProjectileClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "MG|Shooting|Projectile")
	TSubclassOf<AMGProjectile> SemiChargedProjectileClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "MG|Shooting|Projectile")
	TSubclassOf<AMGProjectile> ChargedProjectileClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "MG|Shooting")
	float ShootingStateTime = 0.4;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "MG|Shooting")
	float CurrentShootingStateTime = 0.f;

	UPROPERTY(Transient)
	FTimerHandle ShootingStateTimerHandle;

	UPROPERTY(Transient, VisibleAnywhere, Category = "MG|State")
	bool bIsShooting = false;
};
