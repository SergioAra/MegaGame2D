// Sergio Arana


#include "Controller/MGAIController.h"
#include "Components/StateTreeAIComponent.h"
#include "Character/MGEnemy.h"

AMGAIController::AMGAIController()
{
	StateTreeAIComponent = CreateDefaultSubobject<UStateTreeAIComponent>(TEXT("StateTreeAIComponent"));
}

void AMGAIController::BeginPlay()
{
	Super::BeginPlay();
}

void AMGAIController::OnPossess(APawn * InPawn)
{
	Super::OnPossess(InPawn);

	if (AMGEnemy* Enemy = Cast<AMGEnemy>(InPawn))
	{
		StateTreeAIComponent->SetStateTree(Enemy->GetStateTree());
		StateTreeAIComponent->StartLogic();
	}
}
