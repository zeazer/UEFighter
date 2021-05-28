// Fill out your copyright notice in the Description page of Project Settings.


#include "Hurtbox.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SceneComponent.h"

AHurtbox::AHurtbox()
{
	PrimaryActorTick.bCanEverTick = true;
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));

	mHurtboxDisplay = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("HurtboxDisplay"));
	mHurtboxDisplay->SetupAttachment(this->RootComponent);
}