// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/MGPlayerCharacter.h"


#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"

AMGPlayerCharacter::AMGPlayerCharacter()
{

	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComponent"));
	SpringArmComponent->SetupAttachment(RootComponent);

	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComponent"));
	CameraComponent->SetupAttachment(SpringArmComponent);
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

void AMGPlayerCharacter::StartInputJump()
{
	Jump();
}
