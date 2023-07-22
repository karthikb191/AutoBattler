// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "Data/DALevelGenerator.h"
#include "AutoBattlerGameModeBase.generated.h"

class UGameSubsystem;
/**
 * 
 */
UCLASS()
class AUTOBATTLER_API AAutoBattlerGameModeBase : public AGameModeBase
{
	GENERATED_BODY()
	
protected:

	virtual void InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage) override;
	virtual void StartPlay() override;

	UPROPERTY(EditDefaultsOnly)
	UDALevelGenerator*			LevelGeneratorData;

	UPROPERTY(Transient)
	UGameSubsystem*				GameSubsystem;
};
