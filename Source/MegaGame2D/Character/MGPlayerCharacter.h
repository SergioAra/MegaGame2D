// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/MG2DCharacter.h"
#include "MGPlayerCharacter.generated.h"

class UCameraComponent;
class USpringArmComponent;
class AMGProjectile;
class AMGPlayerController;
class AMGEnemy;
class UAsyncRootMovement;

UCLASS()
class MEGAGAME2D_API AMGPlayerCharacter : public AMG2DCharacter
{
	GENERATED_BODY()

public:

	AMGPlayerCharacter();

	UFUNCTION(BlueprintCallable)
	FORCEINLINE bool IsShooting() const { return bIsShooting; }
	FORCEINLINE bool SetIsShooting(bool bInIsShooting) { return bIsShooting = bInIsShooting; }

	void RestoreEnergy(int32 Amount);

	void InputMove(const FVector2D& AxisValue);
	void UpdateControlRotation();
	void StartInputJump();
	void GroundJump();
	void WallJump();
	void CancelInputJump();
	void InputShootPressed();
	void InputShootHeld(float InHoldTime);
	void InputShootReleased(float InHoldTime);
	void InputSlide();

	void SpawnProjectile(TSubclassOf<AMGProjectile>& ProjectileClassToSpawn);

	virtual void PossessedBy(AController* NewController) override;
	virtual void UnPossessed() override;

	virtual void OnDamageTaken(AActor* DamageSource) override;

	virtual void OnWalkingOffLedge_Implementation(const FVector& PreviousFloorImpactNormal, const FVector& PreviousFloorContactNormal, const FVector& PreviousLocation, float TimeDelta) override;

	void StartStun();

	UFUNCTION()
	void EndStun();

	void StartInvincibility();

	UFUNCTION()
	void EndInvincibility();

	UFUNCTION(BlueprintImplementableEvent)
	void OnEndInvincibility();

	UFUNCTION(BlueprintImplementableEvent)
	void OnInvincibilityVFX();

	UFUNCTION(BlueprintImplementableEvent)
	void OnFlashPartialCharge();

	UFUNCTION(BlueprintImplementableEvent)
	void OnFlashFullCharge();

protected:

	virtual void BeginPlay() override;

	UFUNCTION()
	void OnShootingStateFinished();

	UFUNCTION()
	void OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UFUNCTION()
	virtual void LowTick();

	void CheckInputSlideCancel(const float HorizontalMovementProd);

	bool CheckPressingAgainsWall(const float HorizontalMovementProd, const FVector& CurrentForwardVector);

	void RemoveFallingLateralFriction(float InTime);

	UFUNCTION()
	void ResetFallingLateralFriction();

	UFUNCTION()
	void OnSlideComplete();

	UFUNCTION()
	void OnSlideFailed();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "MG")
	TObjectPtr<USpringArmComponent> SpringArmComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "MG")
	TObjectPtr<UCameraComponent> CameraComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "MG|Shooting|Projectile")
	TSubclassOf<AMGProjectile> ProjectileClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "MG|Shooting|Projectile")
	TSubclassOf<AMGProjectile> PartialChargeProjectileClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "MG|Shooting|Projectile")
	float PartialChargeProjectileTime = 0.8f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "MG|Shooting|Projectile")
	TSubclassOf<AMGProjectile> ChargedProjectileClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "MG|Shooting|Projectile")
	float ChargedProjectileTime = 1.8f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "MG|Shooting")
	float ShootingStateTime = 0.4f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "MG|Shooting")
	float ChargeFlashInterval = 0.2f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "MG|Stats")
	int32 MaxEnergy = 3;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "MG|Stats")
	float InvincibilityDuration = 0.5f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "MG|Stats")
	float StunDuration = 0.4f;

	UPROPERTY(Transient, VisibleAnywhere, BlueprintReadOnly, Category = "MG|Shooting")
	float CurrentShootingStateTime = 0.f;

	UPROPERTY(Transient, VisibleAnywhere, BlueprintReadOnly, Category = "MG|Shooting")
	float CurrentChargeFlashTime = 0.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "MG|VFX")
	float InvincibilityFlickerInterval = 0.12f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "MG|VFX")
	float PartialChargeVFXPlayRate = 8.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "MG|VFX")
	float PartialChargeVFXScale = 10.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "MG|VFX")
	float FullChargeVFXPlayRate = 8.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "MG|VFX")
	float FullChargeVFXScale = 10.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "MG|Knockback")
	float DamageHorizontalLaunchForce = 1200.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "MG|Knockback")
	float DamageVerticalLaunchForce = 1200.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "MG|WallSlide")
	float WallCheckDistance = 200.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "MG|WallSlide")
	float WallSlideMaxSpeed = 20.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "MG|WallSlide")
	float WallJumpHorizontalLaunchForce = 1200.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "MG|WallSlide")
	float WallJumpVerticalLaunchForce = 1200.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "MG|WallSlide")
	float WallJumpImpulseDuration = 0.5f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "MG|GroundSlide")
	float GroundSlideDuration = 0.3f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "MG|GroundSlide")
	float GroundSlideStrength = 1000.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "MG|GroundSlide")
	bool bCancelGroundSlideOnLedges = false;

	UPROPERTY(Transient)
	FTimerHandle ShootingStateTimerHandle;

	UPROPERTY(Transient)
	FTimerHandle InvincibleStateTimerHandle;

	UPROPERTY(Transient)
	FTimerHandle StunStateTimerHandle;

	UPROPERTY(Transient)
	FTimerHandle FallingLateralFrictionResetTimerHandle;

	UPROPERTY(Transient)
	TObjectPtr<UAsyncRootMovement> AsyncSlideMovement = nullptr;

	UPROPERTY(Transient, VisibleAnywhere, BlueprintReadOnly, Category = "MG|State")
	TObjectPtr<AMGPlayerController> PlayerController = nullptr;

	UPROPERTY(Transient, VisibleAnywhere, BlueprintReadOnly, Category = "MG|State")
	bool bIsShooting = false;

	UPROPERTY(Transient, VisibleAnywhere, BlueprintReadOnly, Category = "MG|State")
	bool bIsStunned = false;

	UPROPERTY(Transient, VisibleAnywhere, BlueprintReadOnly, Category = "MG|State")
	bool bIsWallSliding = false;

	UPROPERTY(Transient, VisibleAnywhere, BlueprintReadOnly, Category = "MG|State")
	bool bCancelNextCharge = false;

	UPROPERTY(Transient, VisibleAnywhere, BlueprintReadOnly, Category = "MG|State")
	bool bIsGroundSliding = false;

	UPROPERTY(Transient, VisibleAnywhere, BlueprintReadOnly, Category = "MG|State")
	int32 CurrentEnergy = 0;

	UPROPERTY(Transient, VisibleAnywhere, BlueprintReadOnly, Category = "MG|State")
	TArray<AMGEnemy*> OverlappingEnemies;

	UPROPERTY(Transient, VisibleAnywhere, BlueprintReadOnly, Category = "MG|State")
	float DefaultFallingLateralFriction = 0.f;

	UPROPERTY(Transient, VisibleAnywhere, BlueprintReadOnly, Category = "MG|State")
	FVector2D LastInputDir = FVector2D::ZeroVector;
};
