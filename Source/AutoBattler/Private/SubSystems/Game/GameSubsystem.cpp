// Fill out your copyright notice in the Description page of Project Settings.

#include "SubSystems/Game/GameSubsystem.h"
#include "Level/LevelGenerator.h"
#include "Synchronizer.h"

void UGameSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{

}

void UGameSubsystem::InitLevel(const FLevelGenerationInfo& LevelInfo)
{
	//TODO: Spawn level generator depending on the type of info passed from data asset
	if (LevelGenerator != nullptr)
	{
		LevelGenerator->BeginDestroy();
	}
	LevelGenerator = NewObject<ULevelGenerator>(this, ULevelGenerator::StaticClass());
	LevelGenerator->Initialize(LevelInfo);

	FActorSpawnParameters spawnParams;
	
	spawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	Synchronizer = (ASynchronizer*)GetWorld()->SpawnActor<ASynchronizer>(ASynchronizer::StaticClass(), spawnParams);

	check(Synchronizer != nullptr);
}

void UGameSubsystem::MakeBattlePreparations()
{
	LevelGenerator->GenerateLevel();
	
	TDelegate<void(uint32)> del;
	del.BindUObject(this, &UGameSubsystem::Synchronize);

	Synchronizer->GetTickCallback().Add(del);
}

void UGameSubsystem::PostBattleCleanup()
{
	LevelGenerator->DestroyLevel();
	Synchronizer->Stop();
}

void UGameSubsystem::BeginBattle()
{
	Synchronizer->Start();
}

void UGameSubsystem::EndBattle()
{
	Synchronizer->Stop();
}

void UGameSubsystem::Synchronize(const uint32 timeStamp)
{

}