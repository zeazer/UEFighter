// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "HitboxActor.h"
#include "UEFighterAnimInstance.generated.h"

class AUEFighterCharacter;

UCLASS()
class UEFIGHTER_API UUEFighterAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:
	UUEFighterAnimInstance();

	UFUNCTION(BlueprintCallable)
	void CreateActiveHitbox(UPARAM() FHitBoxData hitboxData);

	UFUNCTION(BlueprintCallable)
	void CreateProximityHitbox(UPARAM() FHitBoxData hitboxData);

	UFUNCTION(BlueprintCallable)
	void DeleteActiveHitboxes();

	UFUNCTION(BlueprintCallable)
	void DeleteProximityHitboxes();

	UPROPERTY(EditAnywhere, BlueprintReadWrite,Category = "DEFAULT")
	AUEFighterCharacter* mCharacterRef;

protected:
	AHitboxActor* CreateHitbox(FHitBoxData& hitboxData);

	UPROPERTY(EditAnywhere, Category = "Player Reference")
	FTransform mPlayerTransform;

	UPROPERTY(EditAnywhere, Category = "Hitboxes")
	TArray<AHitboxActor*> mActiveHitboxes;
	UPROPERTY(EditAnywhere, Category = "Hitboxes")
	TArray<AHitboxActor*> mProximityHitboxes;

	UPROPERTY(EditAnywhere, Category = "Hitboxes")
	TSubclassOf<AHitboxActor> mHitboxActorClass;
};
