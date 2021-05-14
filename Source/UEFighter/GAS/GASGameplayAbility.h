// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "GASGameplayAbility.generated.h"

UENUM(BlueprintType)
enum class EGASAbilityInputID : uint8
{
	None,
	Cancel,
	Confirm,
	Attack1,
	Attack2,
	Attack3,
	Attack4
};

/**
 * 
 */
UCLASS()
class UEFIGHTER_API UGASGameplayAbility : public UGameplayAbility
{
	GENERATED_BODY()

public:
	UGASGameplayAbility();

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Ability")
	EGASAbilityInputID mAbilityInputID = EGASAbilityInputID::None;
};
