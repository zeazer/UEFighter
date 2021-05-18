// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Camera/CameraActor.h"
#include "UEFightCamera.generated.h"

/**
 * 
 */
UCLASS()
class UEFIGHTER_API AUEFightCamera : public ACameraActor
{
	GENERATED_BODY()

public:
	AUEFightCamera();

protected:
	virtual void BeginPlay() override;

	virtual void Tick(float DeltaSeconds) override;

	UFUNCTION()
	void SetPivotActors();
private:
	AActor* mPivotActor1;
	AActor* mPivotActor2;
};
