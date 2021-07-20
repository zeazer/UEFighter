// Fill out your copyright notice in the Description page of Project Settings.


#include "CaptureModel.h"
#include "Components/SkeletalMeshComponent.h"

ACaptureModel::ACaptureModel()
{
	PrimaryActorTick.bCanEverTick = false;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));

	mCharacterSkin = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Character Skin"));
	mCharacterSkin->SetRelativeLocation(FVector::ZeroVector);
}