// Fill out your copyright notice in the Description page of Project Settings.


#include "Controller/MGPlayerController.h"
#include "Controller/MGInputData.h"
#include "Character/MGPlayerCharacter.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"

void AMGPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());

	if (Subsystem)
	{
		Subsystem->ClearAllMappings();
		Subsystem->AddMappingContext(BaseInputMapping, 0);
#if !UE_BUILD_SHIPPING
		Subsystem->AddMappingContext(DeveloperInputMapping, 111);
#endif
	}

	UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(InputComponent);

	if (EnhancedInputComponent && InputActions)
	{
		EnhancedInputComponent->BindAction(InputActions->Move, ETriggerEvent::Triggered, this, &AMGPlayerController::InputMove);
		EnhancedInputComponent->BindAction(InputActions->Move, ETriggerEvent::Completed, this, &AMGPlayerController::InputMove); // Added completed event since the triggered does not fire when releasing the stick
		EnhancedInputComponent->BindAction(InputActions->Jump, ETriggerEvent::Triggered, this, &AMGPlayerController::InputJump);
		EnhancedInputComponent->BindAction(InputActions->Shoot, ETriggerEvent::Triggered, this, &AMGPlayerController::InputShoot);

#if !UE_BUILD_SHIPPING
		// Input bindings for developer Mapping
#endif
	}

}

void AMGPlayerController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
	PlayerCharacter = Cast<AMGPlayerCharacter>(InPawn);
}

void AMGPlayerController::InputMove(const FInputActionValue& Value)
{
	if (PlayerCharacter)
	{
		const FVector2D Axis2DValue = Value.Get<FVector2D>();

		PlayerCharacter->InputMove(Axis2DValue);
	}
}

void AMGPlayerController::InputJump(const FInputActionValue & Value)
{
	const bool bJumping = Value.Get<bool>();
	if (PlayerCharacter)
	{
		if (bJumping)
		{
			PlayerCharacter->StartInputJump();
		}
		else
		{
			PlayerCharacter->CancelInputJump();
		}
	}
}

void AMGPlayerController::InputShoot(const FInputActionValue& Value)
{
	if (PlayerCharacter)
	{
		PlayerCharacter->InputShoot();
	}
}
