// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Hurtbox.generated.h"
class UStaticMeshComponent;

UCLASS()
class UEFIGHTER_API AHurtbox : public AActor
{
	GENERATED_BODY()
	
public:	
	AHurtbox();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Hurtbox")
	UStaticMeshComponent* mHurtboxDisplay;
};
