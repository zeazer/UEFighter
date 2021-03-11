// Fill out your copyright notice in the Description page of Project Settings.


#include "HitboxActor.h"
#include "Engine/StaticMesh.h"
#include "Materials/Material.h"

AHitboxActor::AHitboxActor()
{
	PrimaryActorTick.bCanEverTick = true;
	mHitboxMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("HitboxMesh"));
	if (mHitboxMeshComponent)
	{
		mHitboxMeshComponent->SetupAttachment(RootComponent);
		mHitboxMeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		mHitboxMeshComponent->CanCharacterStepUpOn = ECanBeCharacterBase::ECB_No;
		mHitboxMeshComponent->SetGenerateOverlapEvents(false);
	}
}

void AHitboxActor::SpawnHitbox()
{
	if (mHitboxMeshComponent)
	{
		switch (mHitboxType)
		{
		case EHitboxType::HB_PROXIMITY:
			mHitboxMeshComponent->SetMaterial(0, mGreenMaterial);
			break;
		case EHitboxType::HB_STRIKE:
			mHitboxMeshComponent->SetMaterial(0, mYellowMaterial);
			break;
		case EHitboxType::HB_HURTBOX:
			mHitboxMeshComponent->SetMaterial(0, mRedMaterial);
			break;

		}

		mHitboxMeshComponent->SetWorldLocation(mHitboxLocation);
		mHitboxMeshComponent->SetVisibility(true);
	}
}

void AHitboxActor::BeginPlay()
{
	Super::BeginPlay();
	if (mHitboxMeshComponent)
	{
		mHitboxMeshComponent->SetVisibility(false);
	}
}

// Called every frame
void AHitboxActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AHitboxActor::TriggerVisualizeHitbox()
{
}

