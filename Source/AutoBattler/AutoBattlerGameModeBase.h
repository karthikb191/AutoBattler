// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "Data/DALevelGenerator.h"
#include "Data/DABattleInfo.h"
#include "AutoBattlerGameModeBase.generated.h"

class UGameSubsystem;
class UBattleSubsystem;
/**
 * 
 */
UCLASS()
class AUTOBATTLER_API AAutoBattlerGameModeBase : public AGameModeBase
{
	GENERATED_BODY()
	

public:
	float GetSimulationRate() { return SimulationRate; }
protected:

	virtual void InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage) override;
	virtual void StartPlay() override;

	void StartBattle();
	void HandleBattleFinished();

	UPROPERTY(EditDefaultsOnly)
	UDALevelGenerator*			LevelGeneratorData; 
	UPROPERTY(EditDefaultsOnly)
	UDABattleInfo*				BattleInfoData;

	UPROPERTY(Transient)
	UGameSubsystem*				GameSubsystem;
	UPROPERTY(Transient)
	UBattleSubsystem*			BattleSubsystem;

	/*This specified how often synchronizer ticks*/
	UPROPERTY(EditDefaultsOnly)
	float						SimulationRate = 0.1f;
};
