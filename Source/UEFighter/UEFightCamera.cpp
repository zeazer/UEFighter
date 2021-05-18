// Fill out your copyright notice in the Description page of Project Settings.


#include "UEFightCamera.h"
#include "Camera/CameraComponent.h"
#include "UEFighterGameInstance.h"
#include "Engine/World.h"
#include "TimerManager.h"
#include "UEFighterCharacter.h"
#include "Kismet/GameplayStatics.h"

AUEFightCamera::AUEFightCamera()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AUEFightCamera::BeginPlay()
{
	Super::BeginPlay();
	SetPivotActors();
}

void AUEFightCamera::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if (mPivotActor1 && mPivotActor2)
	{
		FVector pivotLocationSum;

		pivotLocationSum += mPivotActor1->GetActorLocation();
		pivotLocationSum += mPivotActor2->GetActorLocation();

		auto location = GetActorLocation();
		location.Y = pivotLocationSum.Y;
		location.Y /= 2.0f;
		location.Z = pivotLocationSum.Z;
		location.Z /= 2.0f;

		SetActorLocation(location);



		float distance = (mPivotActor1->GetActorLocation() - mPivotActor2->GetActorLocation()).Size() * 0.1f;
		auto fov = FMath::Clamp(FMath::Abs(distance), 60.0f, 85.0f);

		GetCameraComponent()->SetFieldOfView(fov);
	}
}

void AUEFightCamera::SetPivotActors()
{
	auto* gameInstance = Cast<UUEFighterGameInstance>(GetGameInstance());
	if (gameInstance->mPlayer1 && gameInstance->mPlayer2)
	{
		mPivotActor1 = gameInstance->mPlayer1;
		mPivotActor2 = gameInstance->mPlayer2;
	}
	else
	{
		FTimerHandle handle;
		GetWorld()->GetTimerManager().SetTimer(handle, this, &AUEFightCamera::SetPivotActors, 0.1f, false);
	}
}
