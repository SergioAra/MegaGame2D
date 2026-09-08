// Sergio Arana

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "MGAIController.generated.h"

class UStateTreeAIComponent;

UCLASS()
class MEGAGAME2D_API AMGAIController : public AAIController
{
	GENERATED_BODY()

public:

	AMGAIController();

	UFUNCTION(BlueprintCallable)
	UStateTreeAIComponent* GetStateTreeAIComponent() const { return StateTreeAIComponent; }

protected:

	virtual void BeginPlay() override;

	virtual void OnPossess(APawn* InPawn) override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UStateTreeAIComponent> StateTreeAIComponent;
	
};
