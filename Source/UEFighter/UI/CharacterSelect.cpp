// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterSelect.h"

void UCharacterSelect::Init()
{
	APlayerController* playercontroller = GetGameInstance()->GetFirstLocalPlayerController();
	if (playercontroller)
	{
		FInputModeGameOnly inputModeData;
		inputModeData.SetConsumeCaptureMouseDown(false);

		playercontroller->SetInputMode(inputModeData);
		playercontroller->bShowMouseCursor = true;
	}
	Super::Init();
}
