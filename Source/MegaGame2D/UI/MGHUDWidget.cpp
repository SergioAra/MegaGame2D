// Sergio Arana


#include "UI/MGHUDWidget.h"
#include "UI/MGHealthWidget.h"
#include "Character/MGPlayerCharacter.h"

void UMGHUDWidget::HUDSetup(AMGPlayerCharacter* InPlayerCharacter)
{
	if (HealthWidget)
	{
		HealthWidget->SetupHealthWidget(InPlayerCharacter->GetHealthComponent());
	}
}
