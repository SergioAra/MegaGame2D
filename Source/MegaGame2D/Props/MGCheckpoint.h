// Sergio Arana

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MGCheckpoint.generated.h"

class UBoxComponent;

UCLASS()
class MEGAGAME2D_API AMGCheckpoint : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMGCheckpoint();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "MG")
	TObjectPtr<UBoxComponent> TriggerComponent;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION()
	void OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UPROPERTY(Transient, VisibleAnywhere, BlueprintReadOnly, Category = "MG|State")
	bool bEnabled = true;

};
