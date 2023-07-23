// Copyright Epic Games, Inc. All Rights Reserved.

#include "AutoBattlerGameModeBase.h"
#include "SubSystems/Game/GameSubsystem.h"
#include "SubSystems/BattleSubsystem.h"

//Bad hack for restrating battle
FTimerHandle Handle;

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
	GetWorldTimerManager().SetTimer(Handle, this, &AAutoBattlerGameModeBase::StartBattle, 1.0f, false);
}

void AAutoBattlerGameModeBase::StartBattle()
{
	TDelegate<void()> del;
	del.BindUObject(this, &AAutoBattlerGameModeBase::HandleBattleFinished);
	BattleSubsystem->GetBattleFinishedCallback().Add(del);

	//TODO: Move this to be controlled from UI
	GameSubsystem->MakeBattlePreparations();
	BattleSubsystem->MakeBattlePreparations();

	GameSubsystem->BeginBattle();
}

void AAutoBattlerGameModeBase::HandleBattleFinished()
{
	GameSubsystem->PostBattleCleanup();
	BattleSubsystem->PostBattleCleanup();

	GetWorldTimerManager().SetTimer(Handle, this, &AAutoBattlerGameModeBase::StartBattle, 5.0f, false);
}