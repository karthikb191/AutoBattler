// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Templates/UniquePtr.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "GameSubsystem.generated.h"

class ULevelGenerator;
struct FLevelGenerationInfo;
/**
 * 
 */
UCLASS()
class AUTOBATTLER_API UGameSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()
public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;

	void InitLevel(const FLevelGenerationInfo& Levelinfo);
	void GenerateLevel();
protected:

private:
	UPROPERTY(Transient)
	ULevelGenerator*			LevelGenerator = nullptr;
};
