// Fill out your copyright notice in the Description page of Project Settings.


#include "HitboxActor.h"
#include "Engine/StaticMesh.h"
#include "Materials/Material.h"
#include "UEFighterCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "Hurtbox.h"

AHitboxActor::AHitboxActor()
{
	PrimaryActorTick.bCanEverTick = true;
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));

	mHitboxMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("HitboxMesh"));
	if (mHitboxMeshComponent)
	{
		mHitboxMeshComponent->SetupAttachment(RootComponent);
		mHitboxMeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		mHitboxMeshComponent->CanCharacterStepUpOn = ECanBeCharacterBase::ECB_No;
		mHitboxMeshComponent->SetGenerateOverlapEvents(false);
	}
}

void AHitboxActor::SpawnHitbox(const FHitBoxData& hitboxData)
{
	mHitboxData = hitboxData;
	if (mHitboxMeshComponent && GetOwner())
	{
		switch (mHitboxData.hitboxType)
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

		auto* owner = Cast<AUEFighterCharacter>(GetOwner());
		auto direction = owner->GetFaceDirection();
		FVector tempHitboxLocation = mHitboxData.hitboxLocation;
		tempHitboxLocation.Y -= mHitboxData.hitboxOffsetValue * direction;

		mHitboxMeshComponent->SetWorldLocation(tempHitboxLocation);
		mHitboxMeshComponent->SetVisibility(true);
		CheckCollision(mHitboxData.hitboxDamage, mHitboxData.hitStunTime, mHitboxData.blockStunTime, mHitboxData.pushbackAmount, mHitboxData.launchAmount);
	}
}

void AHitboxActor::CheckCollision(const float hitboxDamage, const float hitstunTime, const float blockstunTime, const float pushbackAmount, const float launchAmount)
{
	if (HasAuthority())
	{
		TArray<AActor*> hurtboxesFound;
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), AHurtbox::StaticClass(), hurtboxesFound);

		for (auto* hurtbox : hurtboxesFound)
		{
			auto* instigator = GetInstigator();
			if (instigator != hurtbox->GetOwner())
			{
				auto overlap = this->IsOverlappingActor(hurtbox);
				if (overlap)
				{
					auto* playerChar = Cast<AUEFighterCharacter>(hurtbox->GetOwner());
					if (playerChar)
					{
						playerChar->TakeAbilityDamage(Cast<AUEFighterCharacter>(instigator), hitboxDamage, hitstunTime, blockstunTime, pushbackAmount, launchAmount);
					}
				}
			}
		}

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

void AHitboxActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AHitboxActor::TriggerVisualizeHitbox()
{
}

