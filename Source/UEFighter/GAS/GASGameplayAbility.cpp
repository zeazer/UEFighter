// Fill out your copyright notice in the Description page of Project Settings.


#include "GASGameplayAbility.h"
#include "UEFighter/UEFighterCharacter.h"

UGASGameplayAbility::UGASGameplayAbility()
{
}

UAnimMontage* UGASGameplayAbility::GetAbilityAnimation()
{
	if (Owner)
	{
		return Owner->GetAnimation(AbilitytID);
	}
	return nullptr;
}
