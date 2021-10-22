// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterSelectElement.h"
#include "../UEFighterGameInstance.h"
#include "Kismet/GameplayStatics.h"

void UCharacterSelectElement::OnClick()
{
	auto* gameInstance = Cast<UUEFighterGameInstance>(GetGameInstance());
	gameInstance->mPlayerDetails[0].mPlayerCharacterChoice = mCharacterClassType;

	gameInstance->SwitchToLevelWithName(TEXT("InGame"), TEXT("UEFighterMap"));
}

bool UCharacterSelectElement::Initialize()
{
	ShownCharacterName = UEnum::GetValueAsString<ECharacterClass>(mCharacterClassType);
	FString Left;
	FString Right;
	ShownCharacterName.Split(TEXT("ECharacterClass::"), &Left, &Right);
	ShownCharacterName = Right;
	return Super::Initialize();
}
