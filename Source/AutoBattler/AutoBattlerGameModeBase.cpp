// Copyright Epic Games, Inc. All Rights Reserved.

#include "AutoBattlerGameModeBase.h"
#include "SubSystems/Game/GameSubsystem.h"

void AAutoBattlerGameModeBase::InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage)
{
	AGameModeBase::InitGame(MapName, Options, ErrorMessage);

	GameSubsystem = GetGameInstance()->GetSubsystem<UGameSubsystem>();
	check(GameSubsystem != nullptr)

	GameSubsystem->InitLevel(LevelGeneratorData->LevelGenerationInfo);
}

void AAutoBattlerGameModeBase::StartPlay()
{
	//TODO: Move this to be controlled from UI
	GameSubsystem->MakeBattlePreparations();
}