// Copyright Epic Games, Inc. All Rights Reserved.

#include "AutoBattlerGameModeBase.h"
#include "SubSystems/Game/GameSubsystem.h"
#include "SubSystems/BattleSubsystem.h"

void AAutoBattlerGameModeBase::InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage)
{
	AGameModeBase::InitGame(MapName, Options, ErrorMessage);

	GameSubsystem = GetGameInstance()->GetSubsystem<UGameSubsystem>();
	check(GameSubsystem != nullptr)
	BattleSubsystem = GetWorld()->GetSubsystem<UBattleSubsystem>();
	check(BattleSubsystem != nullptr);


	GameSubsystem->InitLevel(LevelGeneratorData->LevelGenerationInfo);
	BattleSubsystem->InitLevel(BattleInfoData->BattleInfo);
}

void AAutoBattlerGameModeBase::StartPlay()
{
	Super::StartPlay();

	//TODO: Move this to be controlled from UI
	GameSubsystem->MakeBattlePreparations();
	BattleSubsystem->MakeBattlePreparations();

	GameSubsystem->BeginBattle();
}