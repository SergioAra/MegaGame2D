// Sergio Arana


#include "UI/MGHealthWidget.h"
#include "Component/MGHealthComponent.h"

#include "Components/HorizontalBox.h"
#include "Components/HorizontalBoxSlot.h"
#include "Components/Image.h"
#include "Blueprint/WidgetTree.h"

void UMGHealthWidget::SetupHealthWidget(UMGHealthComponent* InHealthComponent)
{
	if (!InHealthComponent)
	{
		return;
	}

	HealthComponent = InHealthComponent;

	HealthComponent->OnDamageTaken.AddDynamic(this, &UMGHealthWidget::OnDamageTaken);
	HealthComponent->OnHealthDepleated.AddDynamic(this, &UMGHealthWidget::OnHealthDepleated);

	const int32 MaxHealth = HealthComponent->GetMaxHealth();
	const int32 SlotDiff = MaxHealth - HealthImages.Num();

	if (SlotDiff > 0)
	{
		// We need more slots
		for (int32 i = 0; i < SlotDiff; ++i)
		{
			UImage* Image = WidgetTree->ConstructWidget<UImage>(UImage::StaticClass(), FName("HealthImage" + FString::FromInt(i + 1)));
			HealthImages.Add(Image);
			UHorizontalBoxSlot* HealthSlot = HealthBox->AddChildToHorizontalBox(Image);
			HealthSlot->SetPadding(FMargin(1.5f, 0.f, 1.5f, 0.f));
		}
	}
	else if (SlotDiff < 0)
	{
		// We have more slots than needed
		for (int32 i = HealthImages.Num() - 1; i > MaxHealth - 1; --i)
		{
			if (UImage* Image = HealthImages[i])
			{
				HealthBox->RemoveChild(Image);
				HealthImages.RemoveAt(i);
			}
		}
	}

	UpdateHealthWidget(MaxHealth);
}

void UMGHealthWidget::NativeConstruct()
{
	Super::NativeConstruct();
}

void UMGHealthWidget::OnDamageTaken(AActor* DamageSource, int32 CurrentHealth)
{
	UpdateHealthWidget(CurrentHealth);
}

void UMGHealthWidget::OnHealthDepleated()
{
	if (!HealthComponent)
	{
		return;
	}

	HealthComponent->OnDamageTaken.RemoveDynamic(this, &UMGHealthWidget::OnDamageTaken);
	HealthComponent->OnHealthDepleated.RemoveDynamic(this, &UMGHealthWidget::OnHealthDepleated);

	HealthComponent = nullptr;
}

void UMGHealthWidget::UpdateHealthWidget(int32 CurrentHealth)
{
	for (int32 i = 0; i < HealthImages.Num(); ++i)
	{
		if (UImage* Image = HealthImages[i])
		{
			if (i < CurrentHealth)
			{
				Image->SetBrushFromTexture(HealthTexture, true);
			}
			else
			{
				Image->SetBrushFromTexture(HealthTextureDamaged, true);
			}
		}
	}
}
