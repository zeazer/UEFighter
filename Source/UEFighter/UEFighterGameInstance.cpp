// Fill out your copyright notice in the Description page of Project Settings.


#include "UEFighterGameInstance.h"
#include "UI/MenuBase.h"
#include "UObject/ConstructorHelpers.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"
#include "UEFighterCharacter.h"
#include "GameFramework/PlayerStart.h"
#include "UEFightCamera.h"
#include "UEFighterController.h"

UUEFighterGameInstance::UUEFighterGameInstance(const FObjectInitializer& ObjectInitializer)
{
	mCurrentActiveWidget = nullptr;
	mNumberOfPlayers = 2;
	mIsDeviceUsedForMultiplePlayers = false;
	LoadCharacterClass(TEXT("/Game/UEFighter/Blueprints/UEFighterCharacter_BP"));
	LoadCharacterClass(TEXT("/Game/UEFighter/Blueprints/SwatFighterCharacter_BP"));
	LoadCharacterClass(TEXT("/Game/UEFighter/Blueprints/MutantFighterCharacter_BP"));
	mPlayerDetails.Init(FPlayerDetails(), mNumberOfPlayers);

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
	return mLoadedCharacterClasses.Contains(mPlayerDetails[0].mPlayerCharacterChoice); /*&& mLoadedCharacterClasses.Contains(mPlayer2CharacterChoice)*/ // Will be needed when player 2 is implemented to full extent.
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

AUEFighterCharacter* UUEFighterGameInstance::GetPlayer(int32 index)
{
	if (index >= 0 && index < mPlayers.Num())
	{
		return mPlayers[index];

	}
	else
	{
		return nullptr;
	}
}

void UUEFighterGameInstance::SpawnPlayers(UWorld* world)
{
	mPlayers.Empty();

	TArray<AActor*> actors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), APlayerStart::StaticClass(), actors);
	for (int index = 0; actors.Num() < 1; index++)
	{
		if (auto* playerStart = Cast<APlayerStart>(actors[index]))
		{
			mPlayers.Emplace();
			mPlayers[index] = DetermineCharacterClass(playerStart->GetTransform(), mPlayerDetails[index].mPlayerCharacterChoice, world);
			PossessPlayer(mPlayers[index], world, index);
		}
	}
}

void UUEFighterGameInstance::PossessPlayer(AUEFighterCharacter* characterRef, UWorld* world, int32 playerIndex)
{
	if (playerIndex == 0)
	{
		auto* controller = UGameplayStatics::GetPlayerController(world, playerIndex);
		controller->Possess(characterRef);
	}
	else if (mIsDeviceUsedForMultiplePlayers)
	{
		FActorSpawnParameters spawnInfo;
		auto* controller = world->SpawnActor<AUEFighterController>(AUEFighterController::StaticClass(), characterRef->GetTransform(), spawnInfo);
		controller->Possess(characterRef);
	}
	else
	{
		auto* controller = UGameplayStatics::CreatePlayer(world);
		controller->Possess(characterRef);
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