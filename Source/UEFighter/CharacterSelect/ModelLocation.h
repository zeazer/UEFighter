// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "../UEFighterGameInstance.h"
#include "ModelLocation.generated.h"
class UCameraComponent;
class UTextureRenderTarget2D;
class ACaptureModel;
class UStaticMeshComponent;

UCLASS()
class UEFIGHTER_API AModelLocation : public AActor
{
	GENERATED_BODY()
	
public:	
	AModelLocation();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera")
	UCameraComponent* mModelCamera;

	UFUNCTION(BlueprintCallable)
	void SpawnModelOnLocation1(ECharacterClass modelClass);

	UFUNCTION(BlueprintCallable)
	void SpawnModelOnLocation2(ECharacterClass modelClass);

	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	UStaticMeshComponent* mModelLocation1;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Model")
	ACaptureModel* mModel1;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	UStaticMeshComponent* mModelLocation2;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Model")
	ACaptureModel* mModel2;

	TMap<ECharacterClass, TSubclassOf<ACaptureModel>> mCaptureModelTypes;
};
