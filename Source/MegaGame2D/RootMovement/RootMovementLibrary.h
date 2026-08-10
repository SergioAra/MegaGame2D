// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/RootMotionSource.h"
#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "RootMovementLibrary.generated.h"

/**
 *
 */
UCLASS()
class MEGAGAME2D_API URootMovementLibrary : public UBlueprintFunctionLibrary
{
    GENERATED_BODY()

public:
    /**
     * Apply root motion movement to passed Character Movement Component
     */
    UFUNCTION(BlueprintCallable, DisplayName = "Apply Root Motion Constant Force", Category = "RootMovement", meta = (WorldContext = "WorldContext", Keywords = "RootMovement RootMotion Root Motion Movement"))
    static void ApplyRootMotionConstantForce(const UObject* WorldContext, UCharacterMovementComponent* CharacterMovement, FVector WorldDirection, float Strength, float Duration, bool bIsAdditive, UCurveFloat* StrengthOverTime, ERootMotionFinishVelocityMode VelocityOnFinishMode, FVector SetVelocityOnFinish, float ClampVelocityOnFinish, bool bEnableGravity);
};