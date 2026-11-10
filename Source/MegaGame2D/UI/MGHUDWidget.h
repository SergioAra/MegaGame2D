// Sergio Arana

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MGHUDWidget.generated.h"

class UMGHealthWidget;
class AMGPlayerCharacter;

UCLASS()
class MEGAGAME2D_API UMGHUDWidget : public UUserWidget
{
	GENERATED_BODY()

public:

	void HUDSetup(AMGPlayerCharacter* InPlayerCharacter);

protected:

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UMGHealthWidget> HealthWidget;
	
};
