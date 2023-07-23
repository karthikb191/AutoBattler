// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Templates/UniquePtr.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "Data/DALevelGenerator.h"
#include "GameSubsystem.generated.h"

class ULevelGenerator;
class ASynchronizer;
/**
 * 
 */
UCLASS()
class AUTOBATTLER_API UGameSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()
public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;

	UFUNCTION(BlueprintCallable)
	void InitLevel(const FLevelGenerationInfo& Levelinfo);
	UFUNCTION(BlueprintCallable)
	void MakeBattlePreparations();
	UFUNCTION(BlueprintCallable)
	void PostBattleCleanup();

	UFUNCTION(BlueprintCallable)
	void BeginBattle();
	UFUNCTION(BlueprintCallable)
	void EndBattle();
	UFUNCTION()
	void Synchronize(const uint32 timeStamp);

	UFUNCTION()
	ULevelGenerator* GetLevelGenerator() { return LevelGenerator; }
	UFUNCTION()
	ASynchronizer* GetSynchronizer() { return Synchronizer; }

protected:

private:
	UPROPERTY(Transient)
	ULevelGenerator*			LevelGenerator = nullptr;

	UPROPERTY(Transient)
	ASynchronizer*				Synchronizer = nullptr;
};
