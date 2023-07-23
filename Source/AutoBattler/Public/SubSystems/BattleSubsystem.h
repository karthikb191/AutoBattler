// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "Data/DABattleInfo.h"
#include "BattleSubsystem.generated.h"

class UGameSubsystem;
class UCreatureCommander;
class ACreature;
class ATile;

DECLARE_MULTICAST_DELEGATE(BattleFinishedDelegate)

/**
 * 
 */
UCLASS()
class AUTOBATTLER_API UBattleSubsystem : public UWorldSubsystem
{
	GENERATED_BODY()
	
public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;

	void InitLevel(const FBattleInfo& Info);
	void MakeBattlePreparations();
	void Synchronize(uint32 TimeStep);
	void PostBattleCleanup();
	//TODO: Retrieve Info from level generator
	//TODO: Add player director


	//Spawn Logic
	void SpawnCreatures();

	//Movement
	//ACreature* GetClosestEnemyTo(ACreature* Searcher);
	TArray<ATile*> TraceCreaturePath(ACreature* From, ACreature* To);

	BattleFinishedDelegate& GetBattleFinishedCallback() { return BattleFinishedCallback; }

private:
	UPROPERTY(Transient)
	UGameSubsystem*			GameSubsystem;
	UPROPERTY(Transient)
	FBattleInfo				BattleInfo;
	//UPROPERTY(Transient)
	//UCreatureCommander*		CreatureCommander;

	//TODO: Add multiple enemies
	UPROPERTY(Transient)
	TArray<ACreature*>		TeamA;
	UPROPERTY(Transient)
	TArray<ACreature*>		TeamB;

	//UPROPERTY(Transient)
	//ACreature*				CreatureA;
	//UPROPERTY(Transient)
	//ACreature*				CreatureB;

	BattleFinishedDelegate	BattleFinishedCallback;
	bool					bBattleRunning;
};
