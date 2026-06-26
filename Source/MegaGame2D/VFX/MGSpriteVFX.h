// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MGSpriteVFX.generated.h"

class UPaperFlipbookComponent;

UCLASS()
class MEGAGAME2D_API AMGSpriteVFX : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMGSpriteVFX();

	/** Please add a variable description */
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Default")
	TObjectPtr<UPaperFlipbookComponent> SpriteComponent;


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION()
	void OnFinishedPlaying();
};
