// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "HitboxActor.generated.h"
class UStaticMeshComponent;
class UMaterial;

UENUM(BlueprintType)
enum class EHitboxType : uint8
{
	HB_PROXIMITY	UMETA(DisplayName = "Proximity"),
	HB_STRIKE		UMETA(DisplayName = "Strike"),
	HB_HURTBOX		UMETA(DisplayName = "HurtBox")
};

UCLASS()
class UEFIGHTER_API AHitboxActor : public AActor
{
	GENERATED_BODY()
	
public:	
	AHitboxActor();

	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable)
	void TriggerVisualizeHitbox();

	UFUNCTION(BlueprintImplementableEvent)
	void VisualizeHitbox();

	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* mHitboxMeshComponent;

	UPROPERTY(EditAnywhere)
	UMaterial* mRedMaterial;

	UPROPERTY(EditAnywhere)
	UMaterial* mGreenMaterial;

	UPROPERTY(EditAnywhere)
	UMaterial* mYellowMaterial;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Hitbox")
	EHitboxType mHitboxType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Hitbox")
	FVector mHitboxLocation;

	UFUNCTION(BlueprintCallable)
	void SpawnHitbox();

protected:
	virtual void BeginPlay() override;



};
