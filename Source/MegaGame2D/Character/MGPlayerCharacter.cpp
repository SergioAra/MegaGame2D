// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/MGPlayerCharacter.h"
#include "Projectile/MGProjectile.h"
#include "PaperFlipbookComponent.h"

#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

static FName MuzzleSocketName(TEXT("Muzzle"));

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

void AMGPlayerCharacter::InputMove(const FVector2D& AxisValue)
{
	AddMovementInput(FVector(AxisValue.X, -AxisValue.Y, 0.f));

	if (AxisValue.X != 0.f)
	{
		const float Yaw = AxisValue.X > 0.f ? 0.f : 180.f;
		GetController()->SetControlRotation(FRotator(0.f, Yaw, 0.f));
	}
}

void AMGPlayerCharacter::CancelInputJump()
{
	StopJumping();
}

void AMGPlayerCharacter::InputShoot()
{
	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	SpawnParams.Owner = this;
	SpawnParams.Instigator = this;

	FVector ShootLocation = GetActorLocation();
	UPaperFlipbookComponent* SpriteComponent = GetSprite();
	if(SpriteComponent && SpriteComponent->DoesSocketExist(MuzzleSocketName))
	{
		ShootLocation = SpriteComponent->GetSocketLocation(MuzzleSocketName);
	}
	else
	{
		// Try to offset the shoot location to the front of the character if no socket is found
		ShootLocation += GetActorForwardVector() * 60.f + GetActorUpVector() * -4.5f;
	}

	DrawDebugSphere(GetWorld(), ShootLocation, 5.f, 12, FColor::Red, false, 1.f);
	GetWorld()->SpawnActor<AMGProjectile>(ProjectileClass, ShootLocation, GetActorRotation(), SpawnParams);


	SetIsShooting(true);
	GetWorldTimerManager().SetTimer(ShootingStateTimerHandle, this, &AMGPlayerCharacter::OnShootingStateFinished, ShootingStateTime);

}

void AMGPlayerCharacter::OnShootingStateFinished()
{
	SetIsShooting(false);
}

void AMGPlayerCharacter::StartInputJump()
{
	Jump();
}
