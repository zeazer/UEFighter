// Fill out your copyright notice in the Description page of Project Settings.


#include "UEFighterGameInstance.h"
#include "UI/MenuBase.h"
#include "UObject/ConstructorHelpers.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"
#include "UEFighterCharacter.h"
#include "GameFramework/PlayerStart.h"

UUEFighterGameInstance::UUEFighterGameInstance(const FObjectInitializer& ObjectInitializer)
{
	mCurrentActiveWidget = nullptr;
	mPlayer1CharacterChoice = ECharacterClass::SwatFighter;
	mPlayer2CharacterChoice = ECharacterClass::SwatFighter;
	LoadCharacterClass(TEXT("/Game/UEFighter/Blueprints/UEFighterCharacter_BP"));
	LoadCharacterClass(TEXT("/Game/UEFighter/Blueprints/SwatFighterCharacter_BP"));
	LoadCharacterClass(TEXT("/Game/UEFighter/Blueprints/MutantFighterCharacter_BP"));


	static ConstructorHelpers::FClassFinder<UMenuBase> inGameWIdget(TEXT("WidgetBlueprint'/Game/UEFighter/UI/PlayerHUD.PlayerHUD_C'"));
	if (inGameWIdget.Succeeded())
	{
		auto type = inGameWIdget.Class.GetDefaultObject()->mMenuType;

		mMenuWidgetClasses.Add(type, inGameWIdget.Class);
	}

	static ConstructorHelpers::FClassFinder<UMenuBase> chararacterSelectWidget(TEXT("WidgetBlueprint'/Game/UEFighter/UI/CharacterSelect_BP.CharacterSelect_BP_C'"));
	if (chararacterSelectWidget.Succeeded())
	{
		auto type = chararacterSelectWidget.Class.GetDefaultObject()->mMenuType;

		mMenuWidgetClasses.Add(type, chararacterSelectWidget.Class);
	}
}

void UUEFighterGameInstance::Init()
{
	for (const auto& pair : mMenuWidgetClasses)
	{
		UE_LOG(LogTemp, Warning, TEXT("We created class %s"), *pair.Value->GetName());
	}
	mMenuStack.Push(EMenuType::Default);
}

void UUEFighterGameInstance::PushMenu(EMenuType menuType)
{
	if (mMenuStack.Last() != menuType)
	{
		mMenuStack.Push(menuType);
	}
	ShowWidget();
}

void UUEFighterGameInstance::PopMenu()
{
	RemoveCurrentWidget();
	mMenuStack.Pop();
	ShowWidget();
}

void UUEFighterGameInstance::SwitchToLevelWithName(const FString& name, const FString& levelName)
{
	if (!name.IsEmpty())
	{
		auto* world = GetWorld();
		if (world->GetMapName() != levelName)
		{
			if (!IsLevelTransfereAvailable())
			{
				return;
			}
			PopMenu();
			UGameplayStatics::OpenLevel(GetWorld(), *levelName);
		}
	}
}

bool UUEFighterGameInstance::IsLevelTransfereAvailable()
{
	return mLoadedCharacterClasses.Contains(mPlayer1CharacterChoice); /*&& mLoadedCharacterClasses.Contains(mPlayer2CharacterChoice)*/ // Will be needed when player 2 is implemented to full extent.
}

void UUEFighterGameInstance::ShowWidget()
{
	if (mMenuStack.Num() < 1)
	{
		UE_LOG(LogTemp, Warning, TEXT("No menus on the stack"));
		return;
	}
	auto* typeInMap = mMenuWidgetClasses.Find(mMenuStack.Last());

	if (!typeInMap)
	{
		UE_LOG(LogTemp, Warning, TEXT("A menu wasn't created or didn't exist"));
		return;
	}

	auto widgetClass = typeInMap->Get();
	auto* menu = CreateWidget<UMenuBase>(this, widgetClass);
	menu->AddToViewport();

	menu->Init();

	mCurrentActiveWidget = menu;
}

void UUEFighterGameInstance::RemoveCurrentWidget()
{
	mCurrentActiveWidget->RemoveFromParent();
	mCurrentActiveWidget = nullptr;
}

void UUEFighterGameInstance::SpawnPlayers(UWorld* world)
{
	TArray<AActor*> actors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), APlayerStart::StaticClass(), actors);
	for (auto* actor : actors)
	{
		if (auto* playerStart = Cast<APlayerStart>(actor))
		{
			if (playerStart->PlayerStartTag == TEXT("P1"))
			{
				mPlayer1 = DetermineCharacterClass(playerStart->GetTransform(), mPlayer1CharacterChoice, world);
				if (auto* controller = UGameplayStatics::GetPlayerController(world, 0))
				{
					controller->Possess(mPlayer1);
				}
			}
			else if (playerStart->PlayerStartTag == TEXT("P2"))
			{
				mPlayer2 = DetermineCharacterClass(playerStart->GetTransform(), mPlayer2CharacterChoice, world);
				if (auto* controller = UGameplayStatics::GetPlayerController(world, 1))
				{
					controller->Possess(mPlayer2);
				}
			}
		}
	}
}

AUEFighterCharacter* UUEFighterGameInstance::DetermineCharacterClass(const FTransform& spawnTransform, const ECharacterClass& characterClass, UWorld* world)
{
	FActorSpawnParameters spawnInfo;
	auto& charclass = mLoadedCharacterClasses.FindChecked(characterClass);
	auto* player = world->SpawnActor<AUEFighterCharacter>(charclass.Get(), spawnTransform, spawnInfo);
	return player;
}

void UUEFighterGameInstance::LoadCharacterClass(const FString& characterClassPath)
{
	ConstructorHelpers::FClassFinder<AUEFighterCharacter> PlayerPawnBPClass(*characterClassPath);
	if (PlayerPawnBPClass.Succeeded())
	{
		auto& type = PlayerPawnBPClass.Class.GetDefaultObject()->mCharacterClass;
		mLoadedCharacterClasses.Emplace(type, PlayerPawnBPClass.Class);
	}

}