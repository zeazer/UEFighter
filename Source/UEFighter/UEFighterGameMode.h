// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "UEFighterGameMode.generated.h"
class AUEFighterCharacter;

UCLASS(minimalapi)
class AUEFighterGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	AUEFighterGameMode();

	virtual void BeginPlay();
private:
};



