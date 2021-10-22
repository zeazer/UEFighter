// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "GASGameplayAbility.generated.h"

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
	FName AbilitytID = TEXT("None");

	UFUNCTION(BlueprintPure)
	class UAnimMontage* GetAbilityAnimation();

protected:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Ability")
	class AUEFighterCharacter* Owner;
};
