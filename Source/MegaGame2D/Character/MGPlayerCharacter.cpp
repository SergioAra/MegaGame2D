// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/MGPlayerCharacter.h"
#include "Projectile/MGProjectile.h"
#include "Character/MGEnemy.h"
#include "Controller/MGPlayerController.h"
#include "Component/MGHealthComponent.h"
#include "RootMovement/AsyncRootMovement.h"
#include "General/MGGameMode.h"

#include "PaperFlipbookComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/GameplayStatics.h"
#include "PaperZDAnimationComponent.h"
#include "PaperZDAnimInstance.h"
#include "EngineUtils.h" 

static FName AnimationJumpStunName(TEXT("JumpStun"));
static float LowDeltaTime(0.1f);
static FName CharacterCollisionProfileName(TEXT("2DCharacter"));

AMGPlayerCharacter::AMGPlayerCharacter()
{

	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComponent"));
	SpringArmComponent->SetupAttachment(RootComponent);

	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComponent"));
	CameraComponent->SetupAttachment(SpringArmComponent);

	JumpMaxHoldTime = 0.4f;

	UCharacterMovementComponent* MovementComponent = GetCharacterMovement();

	if (MovementComponent)
	{
		MovementComponent->AirControl = 1.f;
		MovementComponent->FallingLateralFriction = 50.f;
	}

	UCapsuleComponent* CapsuleComp = GetCapsuleComponent();
	if (CapsuleComp)
	{
		CapsuleComp->SetCollisionProfileName(CharacterCollisionProfileName);
	}
}

void AMGPlayerCharacter::RestoreEnergy(int32 Amount)
{
	CurrentEnergy = FMath::Min(CurrentEnergy + Amount, MaxEnergy);
}

void AMGPlayerCharacter::InputMove(const FVector2D& AxisValue)
{
	if (bIsStunned)
	{
		return;
	}

	LastInputDir = AxisValue;

	AddMovementInput(FVector(LastInputDir.X, -LastInputDir.Y, 0.f));

	if (bIsGroundSliding)
	{
		return;
	}

	UpdateControlRotation();
}

void AMGPlayerCharacter::UpdateControlRotation()
{
	if (LastInputDir.X != 0.f)
	{
		const float Yaw = LastInputDir.X > 0.f ? 0.f : 180.f;
		if (PlayerController)
		{
			PlayerController->SetControlRotation(FRotator(0.f, Yaw, 0.f));
		}
	}
}

void AMGPlayerCharacter::CancelInputJump()
{
	StopJumping();
}

void AMGPlayerCharacter::InputShootPressed()
{
	bCancelNextCharge = false;
}

void AMGPlayerCharacter::InputShootHeld(float InHoldTime)
{
	if (bCancelNextCharge || bIsStunned || bIsGroundSliding || CurrentEnergy <= 0 || InHoldTime < PartialChargeProjectileTime)
	{
		CurrentChargeFlashTime = 0.f;
	}
	else
	{
		CurrentChargeFlashTime += GetWorld()->GetDeltaSeconds();

		if (CurrentChargeFlashTime >= ChargeFlashInterval)
		{
			CurrentChargeFlashTime = 0.f;
			if (InHoldTime >= ChargedProjectileTime)
			{
				OnFlashFullCharge();
			}
			else
			{
				OnFlashPartialCharge();
			}
		}
	}
}

void AMGPlayerCharacter::InputShootReleased(float InHoldTime)
{
	LastShootHoldTime = InHoldTime;
	ShootProjectile();
}

void AMGPlayerCharacter::InputSlide()
{
	if (bIsStunned)
	{
		return;
	}

	if (bIsWallSliding)
	{
		return;
	}

	if (bIsGroundSliding)
	{
		return;
	}

	UCharacterMovementComponent* MovementComponent = GetCharacterMovement();
	if (!MovementComponent)
	{
		return;
	}

	if (!MovementComponent->IsMovingOnGround())
	{
		return;
	}

	// TODO: Implemet Slide cooldown

	AsyncSlideMovement = UAsyncRootMovement::AsyncRootMovement(this, GetCharacterMovement(), GetActorForwardVector(), GroundSlideStrength, GroundSlideDuration, false, nullptr, ERootMotionFinishVelocityMode::MaintainLastRootMotionVelocity, FVector::ZeroVector, 0.f, true);

	if (AsyncSlideMovement)
	{
		bIsGroundSliding = true;
		bCancelNextCharge = true;
		AsyncSlideMovement->OnComplete.AddDynamic(this, &AMGPlayerCharacter::OnSlideComplete);
		AsyncSlideMovement->OnFail.AddDynamic(this, &AMGPlayerCharacter::OnSlideFailed);
		AsyncSlideMovement->Activate();
	}
}

void AMGPlayerCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);
	PlayerController = Cast<AMGPlayerController>(NewController);
}

void AMGPlayerCharacter::UnPossessed()
{
	Super::UnPossessed();
	PlayerController = nullptr;
}

void AMGPlayerCharacter::OnDamageTaken(AActor* DamageSource)
{
	Super::OnDamageTaken(DamageSource);

	bCancelNextCharge = true;

	StartInvincibility();
	GetWorldTimerManager().SetTimer(InvincibleStateTimerHandle, this, &AMGPlayerCharacter::EndInvincibility, InvincibilityDuration);

	StartStun();

	if (AsyncSlideMovement)
	{
		AsyncSlideMovement->Cancel();
	}

	if (DamageSource)
	{
		RemoveFallingLateralFriction(StunDuration);
		const float LaunchHorizontalDirection = ((GetActorLocation() - DamageSource->GetActorLocation()).X < 0) ? -1.f : 1.f;
		LaunchCharacter(FVector(LaunchHorizontalDirection * DamageHorizontalLaunchForce, 0.f, DamageVerticalLaunchForce), true, true);
	}

}

void AMGPlayerCharacter::StartStun()
{
	UPaperZDAnimationComponent* AnimationComp = GetAnimationComponent();
	if (AnimationComp)
	{
		UPaperZDAnimInstance* AnimInstanceComp = AnimationComp->GetAnimInstance();
		if (AnimInstanceComp)
		{
			AnimInstanceComp->JumpToNode(AnimationJumpStunName);
		}
	}

	GetWorldTimerManager().SetTimer(StunStateTimerHandle, this, &AMGPlayerCharacter::EndStun, StunDuration);

	bIsStunned = true;
}

void AMGPlayerCharacter::EndStun()
{
	bIsStunned = false;
}

void AMGPlayerCharacter::StartInvincibility()
{
	if (HealthComponent)
	{
		HealthComponent->SetIsInvincible(true);
	}

	OnInvincibilityVFX();
}

void AMGPlayerCharacter::EndInvincibility()
{
	OnEndInvincibility();

	if (HealthComponent)
	{
		HealthComponent->SetIsInvincible(false);
	}

	TArray<AMGEnemy*> DamagingEnemies = OverlappingEnemies;

	for (AMGEnemy* Enemy : DamagingEnemies)
	{
		if (Enemy)
		{
			UGameplayStatics::ApplyDamage(this, Enemy->GetContactDamage(), Enemy->GetInstigatorController(), Enemy, nullptr);
		}
	}
}

void AMGPlayerCharacter::SetCheckpoint(AActor* NewCheckpoint)
{
	if (PlayerController)
	{
		PlayerController->StartSpot = NewCheckpoint;
	}
}

void AMGPlayerCharacter::FellOutOfWorld(const UDamageType& dmgType)
{
	if (HealthComponent)
	{
		HealthComponent->KillOwner();
	}
}

void AMGPlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

	CurrentEnergy = MaxEnergy;
	UCapsuleComponent* CapsuleComp = GetCapsuleComponent();
	if (CapsuleComp)
	{
		CapsuleComp->OnComponentBeginOverlap.AddDynamic(this, &AMGPlayerCharacter::OnBeginOverlap);
		CapsuleComp->OnComponentEndOverlap.AddDynamic(this, &AMGPlayerCharacter::OnEndOverlap);
	}

	UCharacterMovementComponent* MovementComponent = GetCharacterMovement();
	if (MovementComponent)
	{
		DefaultFallingLateralFriction = MovementComponent->FallingLateralFriction;
	}

	TActorIterator<AMGGameMode> GameModeIt = TActorIterator<AMGGameMode>(GetWorld());
	if (GameModeIt)
	{
		MGGameMode = *GameModeIt;
	}

	FTimerHandle LowTickTimerHandle;
	GetWorldTimerManager().SetTimer(LowTickTimerHandle, this, &AMGPlayerCharacter::LowTick, LowDeltaTime, true);
}

void AMGPlayerCharacter::OnHealthDepleated()
{
	if (PlayerController)
	{
		PlayerController->RespawnPlayerDelayed();
	}
}

FVector AMGPlayerCharacter::GetShootingDirection() const
{
	return GetActorForwardVector() * (bIsWallSliding ? -1.f : 1.f);
}

void AMGPlayerCharacter::ShootProjectile()
{
	if (bCancelNextCharge)
	{
		bCancelNextCharge = false;
		return;
	}

	if (bIsGroundSliding)
	{
		return;
	}

	if (bIsStunned)
	{
		return;
	}

	if (CurrentEnergy <= 0)
	{
		return;
	}

	TSubclassOf<AMGProjectile> ProjectileClassToSpawn = ProjectileClass;

	if (LastShootHoldTime >= ChargedProjectileTime)
	{
		ProjectileClassToSpawn = ChargedProjectileClass;
	}
	else if (LastShootHoldTime >= PartialChargeProjectileTime)
	{
		ProjectileClassToSpawn = PartialChargeProjectileClass;
	}

	int32 EnergyCost = 1;
	AMGProjectile* DefaultProjectile = ProjectileClassToSpawn.GetDefaultObject();
	if (DefaultProjectile)
	{
		EnergyCost = DefaultProjectile->GetEnergyCost();
	}

	const int32 NewEnergy = CurrentEnergy - EnergyCost;

	if (NewEnergy < 0)
	{
		return;
	}

	CurrentEnergy = NewEnergy;

	SpawnProjectile(ProjectileClassToSpawn);

	SetIsShooting(true);
	GetWorldTimerManager().SetTimer(ShootingStateTimerHandle, this, &AMGPlayerCharacter::OnShootingStateFinished, ShootingStateTime);
}

void AMGPlayerCharacter::OnWalkingOffLedge_Implementation(const FVector& PreviousFloorImpactNormal, const FVector& PreviousFloorContactNormal, const FVector& PreviousLocation, float TimeDelta)
{
	Super::OnWalkingOffLedge_Implementation(PreviousFloorImpactNormal, PreviousFloorContactNormal, PreviousLocation, TimeDelta);

	if (bCancelGroundSlideOnLedges && AsyncSlideMovement)
	{
		AsyncSlideMovement->Cancel();
	}
}

void AMGPlayerCharacter::OnShootingStateFinished()
{
	SetIsShooting(false);
}

void AMGPlayerCharacter::OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	AMGEnemy* Enemy = Cast<AMGEnemy>(OtherActor);
	if (Enemy && Enemy->GetCapsuleComponent() == OtherComp)
	{
		UGameplayStatics::ApplyDamage(this, Enemy->GetContactDamage(), Enemy->GetInstigatorController(), Enemy, nullptr);
		OverlappingEnemies.AddUnique(Enemy);
	}
}

void AMGPlayerCharacter::OnEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	AMGEnemy* Enemy = Cast<AMGEnemy>(OtherActor);
	if (Enemy && Enemy->GetCapsuleComponent() == OtherComp)
	{
		OverlappingEnemies.Remove(Enemy);
	}
}

void AMGPlayerCharacter::LowTick()
{

	const FVector CurrentForwardVector = GetActorForwardVector();
	const float HorizontalMovementProd = CurrentForwardVector.X * LastInputDir.X;

	bIsWallSliding = CheckPressingAgainsWall(HorizontalMovementProd, CurrentForwardVector);

	CheckInputSlideCancel(HorizontalMovementProd);
}

void AMGPlayerCharacter::CheckInputSlideCancel(const float HorizontalMovementProd)
{
	if (bIsGroundSliding && HorizontalMovementProd < 0.f && AsyncSlideMovement)
	{
		AsyncSlideMovement->Cancel();
	}
}

bool AMGPlayerCharacter::CheckPressingAgainsWall(const float HorizontalMovementProd, const FVector& CurrentForwardVector)
{
	if (HorizontalMovementProd <= 0.f)
	{
		return false;
	}

	UCharacterMovementComponent* MovementComponent = GetCharacterMovement();
	if (!MovementComponent)
	{
		return false;
	}

	if (!MovementComponent->IsFalling())
	{
		return false;
	}

	FHitResult OutHitResult;
	const FVector StartLocation = GetActorLocation();
	const FVector EndLocation = StartLocation + CurrentForwardVector * WallCheckDistance;
	FCollisionObjectQueryParams ObjectQueryParams;
	ObjectQueryParams.AddObjectTypesToQuery(ECollisionChannel::ECC_WorldDynamic);
	ObjectQueryParams.AddObjectTypesToQuery(ECollisionChannel::ECC_WorldStatic);
	FCollisionQueryParams QueryParams(FName(TEXT("LowTickWallTrace")), false, this);
	GetWorld()->LineTraceSingleByObjectType(
		OutHitResult,
		StartLocation,
		EndLocation,
		ObjectQueryParams,
		QueryParams
	);

	if (OutHitResult.bBlockingHit)
	{
		// TODO: Might be better to use a custom movement mode for wall sliding, but for now just clamp the falling speed
		MovementComponent->Velocity.Z = FMath::Max(MovementComponent->Velocity.Z, -WallSlideMaxSpeed);
	}

	return OutHitResult.bBlockingHit;
}

void AMGPlayerCharacter::RemoveFallingLateralFriction(float InTime)
{
	if (GetWorldTimerManager().GetTimerRemaining(FallingLateralFrictionResetTimerHandle) > InTime)
	{
		// Timer still active and has more time remaining than the new time
		return;
	}

	UCharacterMovementComponent* MovementComponent = GetCharacterMovement();
	if (MovementComponent)
	{
		MovementComponent->FallingLateralFriction = 0.f;
	}

	GetWorldTimerManager().SetTimer(FallingLateralFrictionResetTimerHandle, this, &AMGPlayerCharacter::ResetFallingLateralFriction, InTime);
}

void AMGPlayerCharacter::ResetFallingLateralFriction()
{
	GetWorldTimerManager().ClearTimer(FallingLateralFrictionResetTimerHandle);

	UCharacterMovementComponent* MovementComponent = GetCharacterMovement();
	if (MovementComponent)
	{
		MovementComponent->FallingLateralFriction = DefaultFallingLateralFriction;
	}
}

void AMGPlayerCharacter::OnSlideComplete()
{
	bIsGroundSliding = false;
	AsyncSlideMovement = nullptr;
}

void AMGPlayerCharacter::OnSlideFailed()
{
	bIsGroundSliding = false;
	AsyncSlideMovement = nullptr;
}

void AMGPlayerCharacter::StartInputJump()
{
	if (bIsWallSliding)
	{
		WallJump();
	}
	else
	{
		GroundJump();
	}
}

void AMGPlayerCharacter::GroundJump()
{
	Jump();

	if (AsyncSlideMovement)
	{
		AsyncSlideMovement->Cancel();
	}
	
}

void AMGPlayerCharacter::WallJump()
{
	RemoveFallingLateralFriction(WallJumpImpulseDuration);
	LaunchCharacter(FVector(GetActorForwardVector().X * -1 * WallJumpHorizontalLaunchForce, 0.f, WallJumpVerticalLaunchForce), true, true);
}
