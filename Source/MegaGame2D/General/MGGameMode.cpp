// Sergio Arana


#include "General/MGGameMode.h"

#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"

AMGGameMode::AMGGameMode()
{

}

void AMGGameMode::BeginPlay()
{
	Super::BeginPlay();
}

void AMGGameMode::LevelCompleted(APlayerController* PlayerController, FName NextLevelName)
{
	if (LevelCompletedWidgetClass)
	{
		UUserWidget* LevelCompletedWidget = CreateWidget<UUserWidget>(PlayerController, LevelCompletedWidgetClass);

		if (LevelCompletedWidget)
		{
			LevelCompletedWidget->AddToViewport();
		}
	}

	FTimerHandle LevelCompletedWaitTimerHandle;

	FTimerDelegate TimerDelegate = FTimerDelegate::CreateWeakLambda(this, [this, NextLevelName]()
		{
			UGameplayStatics::OpenLevel(this, NextLevelName);
		});

	GetWorldTimerManager().SetTimer(LevelCompletedWaitTimerHandle, TimerDelegate, LevelCompletedWaitTime, false);
}
