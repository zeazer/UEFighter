// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "UEFighterAnimInstance.generated.h"


UENUM(BlueprintType)
enum EAttackType
{
	LightAttack,
	MediumAttack,
	HeavyAttack,
	SpecialAttack,
	AbortAttack
};
/**
 * 
 */
UCLASS()
class UEFIGHTER_API UUEFighterAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
	
};
