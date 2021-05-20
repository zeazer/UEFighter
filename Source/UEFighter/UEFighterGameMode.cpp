// Copyright Epic Games, Inc. All Rights Reserved.

#include "UEFighterGameMode.h"
#include "UEFighterCharacter.h"
#include "UObject/ConstructorHelpers.h"
#include "Kismet/GameplayStatics.h"
#include "UEFighterGameInstance.h"
#include "UEFighterController.h"

AUEFighterGameMode::AUEFighterGameMode()
{
	DefaultPawnClass = NULL;
	PlayerControllerClass = AUEFighterController::StaticClass();
}

void AUEFighterGameMode::BeginPlay()
{

	Cast<UUEFighterGameInstance>(GetGameInstance())->SpawnPlayers(GetWorld());
}
