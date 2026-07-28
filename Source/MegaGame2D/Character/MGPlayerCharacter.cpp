// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/MGPlayerCharacter.h"
#include "Projectile/MGProjectile.h"
#include "Character/MGEnemy.h"
#include "Controller/MGPlayerController.h"
#include "Component/MGHealthComponent.h"

#include "PaperFlipbookComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/GameplayStatics.h"
#include "PaperZDAnimationComponent.h"
#include "PaperZDAnimInstance.h"

static FName MuzzleSocketName(TEXT("Muzzle"));
static FName AnimationJumpStunName(TEXT("JumpStun"));

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

	AddMovementInput(FVector(AxisValue.X, -AxisValue.Y, 0.f));

	if (AxisValue.X != 0.f)
	{
		const float Yaw = AxisValue.X > 0.f ? 0.f : 180.f;
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
	if (bCancelNextCharge || bIsStunned || CurrentEnergy <= 0 || InHoldTime < PartialChargeProjectileTime)
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
	if(bCancelNextCharge)
	{
		bCancelNextCharge = false;
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

	if (InHoldTime >= ChargedProjectileTime)
	{
		ProjectileClassToSpawn = ChargedProjectileClass;
	}
	else if (InHoldTime >= PartialChargeProjectileTime)
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

	if(NewEnergy < 0)
	{
		return;
	}

	CurrentEnergy = NewEnergy;

	SpawnProjectile(ProjectileClassToSpawn);

	SetIsShooting(true);
	GetWorldTimerManager().SetTimer(ShootingStateTimerHandle, this, &AMGPlayerCharacter::OnShootingStateFinished, ShootingStateTime);

}

void AMGPlayerCharacter::SpawnProjectile(TSubclassOf<AMGProjectile>& ProjectileClassToSpawn)
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
		// Try to offset the shoot location to the front of the character if no socket is found
		ShootLocation += GetActorForwardVector() * 60.f + GetActorUpVector() * -4.5f;
	}

	//DrawDebugSphere(GetWorld(), ShootLocation, 5.f, 12, FColor::Red, false, 1.f);
	GetWorld()->SpawnActor<AMGProjectile>(ProjectileClassToSpawn, ShootLocation, GetActorRotation(), SpawnParams);
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

	if (DamageSource)
	{
		UCharacterMovementComponent* MovementComponent = GetCharacterMovement();
		if (MovementComponent)
		{
			MovementComponent->FallingLateralFriction = 0.f;
		}
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

	UCharacterMovementComponent* MovementComponent = GetCharacterMovement();
	if (MovementComponent)
	{
		MovementComponent->FallingLateralFriction = DefaultFallingLateralFriction;
	}
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

	for (AMGEnemy* Enemy : OverlappingEnemies)
	{
		if (Enemy)
		{
			UGameplayStatics::ApplyDamage(this, Enemy->GetContactDamage(), Enemy->GetInstigatorController(), Enemy, nullptr);
		}
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

void AMGPlayerCharacter::StartInputJump()
{
	Jump();
}
