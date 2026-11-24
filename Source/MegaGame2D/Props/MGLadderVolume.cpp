// Sergio Arana


#include "Props/MGLadderVolume.h"
#include "Character/MGPlayerCharacter.h"
#include "MG2DDefinitions.h"

#include "Components/BoxComponent.h"
#include "MGLadderVolume.h"

// Sets default values
AMGLadderVolume::AMGLadderVolume()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	TriggerComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("TriggerComponent"));
	TriggerComponent->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	TriggerComponent->SetCollisionResponseToChannel(ECC_Player, ECollisionResponse::ECR_Overlap);

	RootComponent = TriggerComponent;
}

// Called when the game starts or when spawned
void AMGLadderVolume::BeginPlay()
{
	Super::BeginPlay();

	TriggerComponent->OnComponentBeginOverlap.AddDynamic(this, &AMGLadderVolume::OnBeginOverlap);
	TriggerComponent->OnComponentEndOverlap.AddDynamic(this, &AMGLadderVolume::OnEndOverlap);

}

void AMGLadderVolume::OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	AMGPlayerCharacter* PlayerCharacter = Cast<AMGPlayerCharacter>(OtherActor);
	if (PlayerCharacter)
	{
		PlayerCharacter->AddLadder(this);
	}
}

void AMGLadderVolume::OnEndOverlap(UPrimitiveComponent * OverlappedComponent, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex)
{
	AMGPlayerCharacter* PlayerCharacter = Cast<AMGPlayerCharacter>(OtherActor);
	if (PlayerCharacter)
	{
		PlayerCharacter->RemoveLadder(this);
	}
}

