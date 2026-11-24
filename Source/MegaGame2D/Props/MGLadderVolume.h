// Sergio Arana

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MGLadderVolume.generated.h"

class UBoxComponent;

UCLASS()
class MEGAGAME2D_API AMGLadderVolume : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMGLadderVolume();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "MG")
	TObjectPtr<UBoxComponent> TriggerComponent;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION()
	void OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

};
