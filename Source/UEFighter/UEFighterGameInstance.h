// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "UI/MenuType.h"
#include "UEFighterGameInstance.generated.h"
class UUserWidget;
class AUEFighterCharacter;
class AUEFightCamera;

UENUM(BlueprintType)
enum class ECharacterClass : uint8
{
	Default			UMETA(DisplayName = "Default, Manequin"),
	SwatFighter     UMETA(DisplayName = "SwatFighter"),
	MutantFighter   UMETA(DisplayName = "MutantFighter")
};

UCLASS()
class UEFIGHTER_API UUEFighterGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	UUEFighterGameInstance(const FObjectInitializer& ObjectInitializer);

	virtual void Init();

	UFUNCTION(BlueprintCallable)
	void PushMenu(EMenuType menuType);

	UFUNCTION(BlueprintCallable)
	void PopMenu();

	UFUNCTION(BlueprintCallable)
	void SwitchToLevelWithName(const FString& name, const FString& levelName);

#pragma region Player/CharacterVariables

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player References")
		AUEFighterCharacter* mPlayer1;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character Class Choice")
		ECharacterClass mPlayer1CharacterChoice;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player References")
		AUEFighterCharacter* mPlayer2;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character Class Choice")
		ECharacterClass mPlayer2CharacterChoice;
#pragma endregion

	void SpawnPlayers(UWorld* world);

private:
	bool IsLevelTransfereAvailable();

	void ShowWidget();

	void RemoveCurrentWidget();

	TMap<TEnumAsByte<EMenuType>, TSubclassOf<class UMenuBase>> mMenuWidgetClasses;

	TArray<TEnumAsByte<EMenuType>> mMenuStack;

	UUserWidget* mCurrentActiveWidget;



	AUEFighterCharacter* DetermineCharacterClass(const FTransform& spawnTransform, const ECharacterClass& characterClass, UWorld* world);

	void LoadCharacterClass(const FString& characterClassPath);

	TMap<ECharacterClass, TSubclassOf<AUEFighterCharacter>> mLoadedCharacterClasses;
};
