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

USTRUCT(BlueprintType)
struct FHitBoxData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EHitboxType hitboxType = EHitboxType::HB_PROXIMITY;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector hitboxLocation = FVector::ZeroVector;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float hitboxDamage = 0.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float hitboxOffsetValue = 0.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float hitStunTime = 0.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float blockStunTime = 0.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float pushbackAmount = 0.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float launchAmount = 0.f;
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

	UFUNCTION(BlueprintCallable)
	void SpawnHitbox( const FHitBoxData& hitboxData);

	UFUNCTION(BlueprintCallable)
	void CheckCollision(const float hitboxDamage, const float hitstunTime, const float blockstunTime, const float pushbackDistance, const float launchAmount);

	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* mHitboxMeshComponent;

	UPROPERTY(EditAnywhere)
	UMaterial* mRedMaterial;

	UPROPERTY(EditAnywhere)
	UMaterial* mGreenMaterial;

	UPROPERTY(EditAnywhere)
	UMaterial* mYellowMaterial;

protected:
	UPROPERTY(EditAnywhere)
	FHitBoxData mHitboxData;

	virtual void BeginPlay() override;
};