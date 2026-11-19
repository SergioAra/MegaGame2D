// Sergio Arana


#include "Props/MGWinArea.h"
#include "Character/MGPlayerCharacter.h"
#include "MG2DDefinitions.h"
#include "General/MGGameMode.h"

#include "Components/BoxComponent.h"
#include "EngineUtils.h" 

// Sets default values
AMGWinArea::AMGWinArea()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	TriggerComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("TriggerComponent"));
	TriggerComponent->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	TriggerComponent->SetCollisionResponseToChannel(ECC_Player, ECollisionResponse::ECR_Overlap);

}

// Called when the game starts or when spawned
void AMGWinArea::BeginPlay()
{
	Super::BeginPlay();

	TriggerComponent->OnComponentBeginOverlap.AddDynamic(this, &AMGWinArea::OnBeginOverlap);

}

void AMGWinArea::OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!bEnabled)
	{
		return;
	}

	AMGPlayerCharacter* PlayerCharacter = Cast<AMGPlayerCharacter>(OtherActor);
	if (PlayerCharacter)
	{
		// Can only be triggered once
		bEnabled = false;

		TActorIterator<AMGGameMode> GameModeIt = TActorIterator<AMGGameMode>(GetWorld());
		if (GameModeIt)
		{
			AMGGameMode* MGGameMode = *GameModeIt;
			MGGameMode->LevelCompleted(PlayerCharacter->GetController<APlayerController>(), NextLevelName);
		}
	}
}

