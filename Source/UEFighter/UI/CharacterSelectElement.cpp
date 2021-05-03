// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterSelectElement.h"
#include "../UEFighterGameInstance.h"
#include "Kismet/GameplayStatics.h"

void UCharacterSelectElement::OnClick()
{
	auto* gameInstance = Cast<UUEFighterGameInstance>(GetGameInstance());
	gameInstance->mPlayer1CharacterChoice = mCharacterClassType;

	gameInstance->SwitchToLevelWithName(TEXT("InGame"), TEXT("UEFighterMap"));
}
