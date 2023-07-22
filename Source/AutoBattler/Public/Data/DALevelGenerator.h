// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "GameFramework/Actor.h"
#include "Math/Vector2D.h"
#include "Math/IntPoint.h"
#include "Level/Tile.h"
#include "DALevelGenerator.generated.h"

/**
 * 
 */
USTRUCT()
struct FLevelGenerationInfo
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<ATile>	Tile;
	UPROPERTY(EditDefaultsOnly)
	FVector				PlatformCenter;
	UPROPERTY(EditDefaultsOnly)
	FVector2D			TileDimensions;
	UPROPERTY(EditDefaultsOnly)
	FIntPoint			TileCount;
};

UCLASS()
class AUTOBATTLER_API UDALevelGenerator : public UDataAsset
{
	GENERATED_BODY()

public:

	UPROPERTY(EditDefaultsOnly)
	FLevelGenerationInfo	LevelGenerationInfo;
};
