// Sergio Arana

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MGHealthWidget.generated.h"

class UMGHealthComponent;
class UHorizontalBox;
class UTexture2D;
class UImage;

UCLASS()
class MEGAGAME2D_API UMGHealthWidget : public UUserWidget
{
	GENERATED_BODY()

public:

	void SetupHealthWidget(UMGHealthComponent* InHealthComponent);

	void UpdateHealthWidget(int32 CurrentHealth);

protected:

	virtual void NativeConstruct() override;

	UFUNCTION()
	virtual void OnDamageTaken(AActor* DamageSource, int32 CurrentHealth);

	UFUNCTION()
	virtual void OnHealthDepleated();

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UHorizontalBox> HealthBox;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "MG|UI")
	TObjectPtr<UTexture2D> HealthTexture;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "MG|UI")
	TObjectPtr<UTexture2D> HealthTextureDamaged;

	UPROPERTY(Transient, VisibleAnywhere, BlueprintReadOnly, Category = "MG|UI")
	TArray<TObjectPtr<UImage>> HealthImages;

	UPROPERTY(Transient, VisibleAnywhere, BlueprintReadOnly, Category = "MG")
	TObjectPtr<UMGHealthComponent> HealthComponent;
	
};
