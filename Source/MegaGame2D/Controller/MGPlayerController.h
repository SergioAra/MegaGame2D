// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "MGPlayerController.generated.h"

class UInputMappingContext;
class UMGInputData;
class AMGPlayerCharacter;

UCLASS()
class MEGAGAME2D_API AMGPlayerController : public APlayerController
{
	GENERATED_BODY()

protected:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "MG|Input")
	TObjectPtr<UInputMappingContext> BaseInputMapping;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "MG|Input")
	TObjectPtr<UInputMappingContext> DeveloperInputMapping;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "MG|Input")
	TObjectPtr<UMGInputData> InputActions;

	UPROPERTY(Transient, VisibleInstanceOnly, BlueprintReadOnly, Category = "MG")
	TObjectPtr<AMGPlayerCharacter> PlayerCharacter;

	virtual void SetupInputComponent() override;

	virtual void OnPossess(APawn* InPawn) override;

	// **** Inputs Events **** //

	UFUNCTION()
	void InputMove(const FInputActionValue& Value);

	UFUNCTION()
	void InputJump(const FInputActionValue& Value);

	UFUNCTION()
	void InputShoot(const FInputActionValue& Value);
	
};
