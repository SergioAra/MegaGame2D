// Sergio Arana

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "MGGameMode.generated.h"

class UUserWidget;
class APlayerController;

UCLASS()
class MEGAGAME2D_API AMGGameMode : public AGameMode
{
	GENERATED_BODY()

public:
	
	AMGGameMode();

	void BeginPlay() override;

	void LevelCompleted(APlayerController* PlayerController, FName NextLevelName);

protected:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "MG|Level")
	float LevelCompletedWaitTime = 3.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "MG|UI")
	TSubclassOf<UUserWidget> LevelCompletedWidgetClass;
};
