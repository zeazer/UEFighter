// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "../UEFighterGameInstance.h"
#include "CharacterSelectElement.generated.h"

/**
 * 
 */
UCLASS()
class UEFIGHTER_API UCharacterSelectElement : public UUserWidget
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character")
	ECharacterClass mCharacterClassType;

	UFUNCTION(BlueprintCallable)
	void OnClick();
};
