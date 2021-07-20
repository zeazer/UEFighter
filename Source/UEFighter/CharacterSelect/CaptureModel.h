// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "..\UEFighterGameInstance.h"
#include "CaptureModel.generated.h"

UCLASS()
class UEFIGHTER_API ACaptureModel : public AActor
{
	GENERATED_BODY()
	
public:	
	ACaptureModel();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character")
	USkeletalMeshComponent* mCharacterSkin;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character")
	ECharacterClass mCharacterClass;
};
