// Fill out your copyright notice in the Description page of Project Settings.


#include "VFX/MGSpriteVFX.h"

#include "PaperFlipbookComponent.h"

static FName SpriteCollisionProfileName(TEXT("NoCollision"));

// Sets default values
AMGSpriteVFX::AMGSpriteVFX()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	SpriteComponent = CreateDefaultSubobject<UPaperFlipbookComponent>(TEXT("SpriteComponent"));
	SetRootComponent(SpriteComponent);
	SpriteComponent->SetGenerateOverlapEvents(false);
	SpriteComponent->SetCollisionProfileName(SpriteCollisionProfileName);

}

// Called when the game starts or when spawned
void AMGSpriteVFX::BeginPlay()
{
	Super::BeginPlay();

	if (SpriteComponent)
	{
		SpriteComponent->SetLooping(false);
		SpriteComponent->OnFinishedPlaying.AddDynamic(this, &AMGSpriteVFX::OnFinishedPlaying);
	}
}

void AMGSpriteVFX::OnFinishedPlaying()
{
	Destroy();
}

