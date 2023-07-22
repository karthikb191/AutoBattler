// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Data/DALevelGenerator.h"
#include "LevelGenerator.generated.h"


class ATile;
/**
 * 
 */
UCLASS()
class AUTOBATTLER_API ULevelGenerator : public UObject
{
	GENERATED_BODY()
	
public:
	ULevelGenerator();
	~ULevelGenerator();

	void Initialize(const FLevelGenerationInfo& Info);
	
	UFUNCTION()
	void GenerateLevel();
	UFUNCTION()
	void DestroyLevel();

private:
	UPROPERTY(Transient)
	FLevelGenerationInfo			LevelGenerationInfo;
	
	TArray<ATile*>					Tiles;
	bool							bRunning = false;
};
