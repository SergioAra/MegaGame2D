// Sergio Arana


#include "Props/MGCheckpoint.h"
#include "Character/MGPlayerCharacter.h"

#include "Components/BoxComponent.h"

// Sets default values
AMGCheckpoint::AMGCheckpoint()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	TriggerComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("TriggerComponent"));
	TriggerComponent->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	TriggerComponent->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);

}

// Called when the game starts or when spawned
void AMGCheckpoint::BeginPlay()
{
	Super::BeginPlay();

	TriggerComponent->OnComponentBeginOverlap.AddDynamic(this, &AMGCheckpoint::OnBeginOverlap);
	
}

void AMGCheckpoint::OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
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
		//SetActorEnableCollision(false);
		PlayerCharacter->SetCheckpoint(this);
	}
}

