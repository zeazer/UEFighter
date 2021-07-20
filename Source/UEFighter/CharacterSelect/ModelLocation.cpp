// Fill out your copyright notice in the Description page of Project Settings.


#include "ModelLocation.h"
#include "CaptureModel.h"
#include "Camera/CameraComponent.h"
#include "..\UEFighterGameInstance.h"
#include "..\UEFighterCharacter.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/StaticMeshComponent.h"
#include "UObject/ConstructorHelpers.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AModelLocation::AModelLocation()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));

	mModelLocation1 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Model Location 1"));
	mModelLocation1->SetupAttachment(RootComponent);
	mModelLocation1->SetRelativeLocation(FVector::ZeroVector);

	mModelLocation2 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Model Location 2"));
	mModelLocation2->SetupAttachment(RootComponent);
	mModelLocation2->SetRelativeLocation(FVector::ZeroVector);

	mModelCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("Model Camera"));
	mModelCamera->SetupAttachment(RootComponent);
}

void AModelLocation::SpawnModelOnLocation1(ECharacterClass modelClass)
{
	if (auto* world = GetWorld())
	{
		if (mModel1->IsValidLowLevel())
		{
			mModel1->Destroy();
		}

		auto& model= mCaptureModelTypes[modelClass];

		FActorSpawnParameters spawnInfo;
		mModel1 = world->SpawnActor<ACaptureModel>(model.Get(), mModelLocation1->GetComponentTransform(), spawnInfo);
	}
}

void AModelLocation::SpawnModelOnLocation2(ECharacterClass modelClass)
{
	if (auto* world = GetWorld())
	{
		if (mModel2->IsValidLowLevel())
		{
			mModel2->Destroy();
		}

		auto& model = mCaptureModelTypes[modelClass];

		FActorSpawnParameters spawnInfo;
		mModel2 = world->SpawnActor<ACaptureModel>(model.Get(), mModelLocation2->GetComponentTransform(), spawnInfo);
	}
}

void AModelLocation::BeginPlay()
{
	auto& map = Cast<UUEFighterGameInstance>(GetGameInstance())->GetLoadedCharacterClasses();
	TArray<TSubclassOf<AUEFighterCharacter>> classes;
	map.GenerateValueArray(classes);

	for (auto& cl : classes)
	{
		auto charClassType = cl->GetDefaultObject<AUEFighterCharacter>()->GetCharacterClass();
		auto captureModelType = cl->GetDefaultObject<AUEFighterCharacter>()->mCaptureModel;
		mCaptureModelTypes.Add(charClassType, captureModelType);
	}


	APlayerController* PC = UGameplayStatics::GetPlayerController(this, 0);
	if (PC)
	{
		PC->SetViewTarget(this);
	}
}
