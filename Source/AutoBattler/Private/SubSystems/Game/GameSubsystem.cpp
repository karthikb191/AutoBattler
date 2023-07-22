// Fill out your copyright notice in the Description page of Project Settings.

#include "SubSystems/Game/GameSubsystem.h"
#include "Level/LevelGenerator.h"

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
}

void UGameSubsystem::GenerateLevel()
{
	LevelGenerator->GenerateLevel();
}