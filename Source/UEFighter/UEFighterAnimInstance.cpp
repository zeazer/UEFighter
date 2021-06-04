// Fill out your copyright notice in the Description page of Project Settings.


#include "UEFighterAnimInstance.h"
#include "UEFighterCharacter.h"
#include "UObject/ConstructorHelpers.h"

UUEFighterAnimInstance::UUEFighterAnimInstance()
{
	ConstructorHelpers::FClassFinder<AHitboxActor> hitboxActorClass(TEXT("Blueprint'/Game/UEFighter/Blueprints/HitboxActor_BP.HitboxActor_BP_C'"));
	if (hitboxActorClass.Succeeded())
	{
		mHitboxActorClass = hitboxActorClass.Class;
	}
}

void UUEFighterAnimInstance::CreateActiveHitbox(FHitBoxData hitboxData)
{
	auto* hitbox = CreateHitbox(hitboxData);
	DeleteProximityHitboxes();
	mActiveHitboxes.Add(hitbox);
}

void UUEFighterAnimInstance::CreateProximityHitbox(FHitBoxData hitboxData)
{
	auto* hitbox = CreateHitbox(hitboxData);
	mProximityHitboxes.Add(hitbox);
}

void UUEFighterAnimInstance::DeleteActiveHitboxes()
{
	for (auto* hitbox : mActiveHitboxes)
	{
		hitbox->K2_DestroyActor();
	}
	mActiveHitboxes.Empty();
}

void UUEFighterAnimInstance::DeleteProximityHitboxes()
{
	for (auto* hitbox : mProximityHitboxes)
	{
		hitbox->K2_DestroyActor();
	}
	mProximityHitboxes.Empty();
}

AHitboxActor* UUEFighterAnimInstance::CreateHitbox(FHitBoxData& hitboxData)
{
	FActorSpawnParameters spawnInfo;
	auto& transform = mCharacterRef->GetTransform();
	auto* hitbox = GetWorld()->SpawnActor<AHitboxActor>(mHitboxActorClass, transform, spawnInfo);
	hitbox->AttachToComponent(mCharacterRef->GetRootComponent(), FAttachmentTransformRules::KeepWorldTransform);
	hitbox->SetOwner(mCharacterRef);
	hitbox->SetInstigator(mCharacterRef);

	hitboxData.hitboxLocation = transform.GetLocation();
	hitbox->SpawnHitbox(hitboxData);
	return hitbox;
}
