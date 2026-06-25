// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/MG2DCharacter.h"
#include "MGPlayerCharacter.generated.h"

class UCameraComponent;
class USpringArmComponent;

UCLASS()
class MEGAGAME2D_API AMGPlayerCharacter : public AMG2DCharacter
{
	GENERATED_BODY()

public:

	AMGPlayerCharacter();

	void InputMove(const FVector2D& AxisValue);

	void StartInputJump();
	void CancelInputJump();

protected:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "MG", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USpringArmComponent> SpringArmComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "MG", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UCameraComponent> CameraComponent;
	
};
