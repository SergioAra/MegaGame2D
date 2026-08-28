// Fill out your copyright notice in the Description page of Project Settings.


#include "Controller/MGPlayerController.h"
#include "Controller/MGInputData.h"
#include "Character/MGPlayerCharacter.h"
#include "General/MGGameMode.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "EngineUtils.h" 

void AMGPlayerController::RespawnPlayerDelayed()
{
	if (PlayerCameraManager)
	{
		PlayerCameraManager->CameraStyle = "Fixed";
		PlayerCameraManager->StartCameraFade(0.f, 1.f, RespawnDelay, FLinearColor::Black, false, true);
	}

	FTimerHandle RespawnTimerHandle;
	GetWorldTimerManager().SetTimer(RespawnTimerHandle, this, &AMGPlayerController::RespawnPlayer, RespawnDelay, false);

	// Destroying the player here messes up the camera manager, so only disable input and collision
	DisableInput(this);
	if (PlayerCharacter)
	{
		PlayerCharacter->SetActorEnableCollision(false);
		PlayerCharacter->SetActorHiddenInGame(true);
	}

}

void AMGPlayerController::RespawnPlayer()
{
	EnableInput(this);
	if (PlayerCharacter)
	{
		PlayerCharacter->Destroy();
	}

	if (PlayerCameraManager)
	{
		PlayerCameraManager->CameraStyle = NAME_Default;
		PlayerCameraManager->StartCameraFade(1.f, 0.f, RespawnFadeOutDelay, FLinearColor::Black, false, true);
	}
	
	if (MGGameMode)
	{
		MGGameMode->RestartPlayer(this);
	}
}

void AMGPlayerController::BeginPlay()
{
	TActorIterator<AMGGameMode> GameModeIt = TActorIterator<AMGGameMode>(GetWorld());
	if (GameModeIt)
	{
		MGGameMode = *GameModeIt;
	}
}

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
		EnhancedInputComponent->BindAction(InputActions->Shoot, ETriggerEvent::Ongoing, this, &AMGPlayerController::InputShootHeld);
		EnhancedInputComponent->BindAction(InputActions->Slide, ETriggerEvent::Triggered, this, &AMGPlayerController::InputSlide);

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

void AMGPlayerController::InputShoot(const FInputActionInstance& Instance)
{
	const bool bIsShootingHeld = Instance.GetValue().Get<bool>();

	if (bIsShootingHeld)
	{
		if (PlayerCharacter)
		{
			PlayerCharacter->InputShootPressed();
		}
	}
	else
	{
		if (PlayerCharacter)
		{
			PlayerCharacter->InputShootReleased(Instance.GetElapsedTime());
		}
	}
}

void AMGPlayerController::InputShootHeld(const FInputActionInstance& Instance)
{
	if (PlayerCharacter)
	{
		PlayerCharacter->InputShootHeld(Instance.GetElapsedTime());
	}
}

void AMGPlayerController::InputSlide(const FInputActionInstance& Instance)
{
	if (PlayerCharacter)
	{
		PlayerCharacter->InputSlide();
	}
}
