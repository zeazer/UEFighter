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

	mHitboxDamage = 0.f;
	mHitstunTIme = 0.f;
	mBlockstunTime = 0.f;

	mHitboxMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("HitboxMesh"));
	if (mHitboxMeshComponent)
	{
		mHitboxMeshComponent->SetupAttachment(RootComponent);
		mHitboxMeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		mHitboxMeshComponent->CanCharacterStepUpOn = ECanBeCharacterBase::ECB_No;
		mHitboxMeshComponent->SetGenerateOverlapEvents(false);
	}
}

void AHitboxActor::SpawnHitbox(const FVector& hitboxLocation, float hitboxDamage, float hitboxOffsetValue, float stunTime)
{
	if (mHitboxMeshComponent && GetOwner())
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

		auto* owner = Cast<AUEFighterCharacter>(GetOwner());
		auto direction = owner->GetFaceDirection();
		mHitboxLocation = hitboxLocation;
		mHitboxLocation.Y -= hitboxOffsetValue * direction;
		
		mHitboxMeshComponent->SetWorldLocation(mHitboxLocation);
		mHitboxMeshComponent->SetVisibility(true);
		CheckCollision(hitboxDamage, stunTime);
	}
}

void AHitboxActor::CheckCollision(float hitboxDamage, float stunTime)
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
						playerChar->TakeAbilityDamage(hitboxDamage, stunTime);
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

