// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterSelectElement.h"
#include "../UEFighterGameInstance.h"

void UCharacterSelectElement::OnClick()
{
	Cast<UUEFighterGameInstance>(GetGameInstance())->mPlayer1CharacterChoice = mCharacterClassType;
}
