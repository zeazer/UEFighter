// Copyright Epic Games, Inc. All Rights Reserved.

#include "UEFighterGameMode.h"
#include "UEFighterCharacter.h"
#include "UObject/ConstructorHelpers.h"

AUEFighterGameMode::AUEFighterGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/SideScrollerCPP/Blueprints/SideScrollerCharacter"));
	if (PlayerPawnBPClass.Class != nullptr)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
