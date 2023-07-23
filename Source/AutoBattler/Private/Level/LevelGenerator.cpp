// Fill out your copyright notice in the Description page of Project Settings.


#include "Level/LevelGenerator.h"
#include "Level/Tile.h"
#include "Kismet/GameplayStatics.h"
#include "Data/DALevelGenerator.h"

DECLARE_LOG_CATEGORY_EXTERN(LevelGenerator, Warning, Warning)
DEFINE_LOG_CATEGORY(LevelGenerator)

ULevelGenerator::ULevelGenerator()
	: UObject()
{
}
ULevelGenerator::~ULevelGenerator()
{
	if (bRunning)
	{
		DestroyLevel();
	}
}

void ULevelGenerator::Initialize(const FLevelGenerationInfo& Info)
{
	LevelGenerationInfo = Info;
}

void ULevelGenerator::GenerateLevel()
{
	bRunning = true;
	int numItr = LevelGenerationInfo.TileCount.X * LevelGenerationInfo.TileCount.Y;
	Tiles.SetNum(numItr);

	FVector2D TileDimensions = LevelGenerationInfo.TileDimensions;
	float hX = LevelGenerationInfo.TileCount.X * TileDimensions.X / 2.0f;
	float hY = LevelGenerationInfo.TileCount.Y * TileDimensions.Y / 2.0f;

	float currentX = LevelGenerationInfo.PlatformCenter.X - hX + TileDimensions.X / 2.0f;
	float currentY = LevelGenerationInfo.PlatformCenter.Z - hY + TileDimensions.Y / 2.0f;

	FActorSpawnParameters spawnParams;
	spawnParams.bNoFail = true;
	spawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	spawnParams.Owner = GetWorld()->GetFirstPlayerController();

	for (int i = 0; i < numItr; i++)
	{
		if (i % LevelGenerationInfo.TileCount.X == 0)
		{
			currentX = LevelGenerationInfo.PlatformCenter.X - hX - TileDimensions.X / 2.0f;
			currentY += TileDimensions.Y;
		}
		
		FTransform transform = FTransform(FQuat::Identity,
			FVector3d(currentX, currentY, LevelGenerationInfo.PlatformCenter.Z),
			FVector3d(LevelGenerationInfo.TileDimensions.X, TileDimensions.Y, 1.0f));
		
		
		Tiles[i] = (ATile*)GetWorld()->SpawnActor<ATile>(LevelGenerationInfo.Tile.Get(), transform, spawnParams);
		Tiles[i]->SetActorLabel(FString::Printf(TEXT("Tile_%d.%d "), i % LevelGenerationInfo.TileCount.X, i / LevelGenerationInfo.TileCount.X));
		Tiles[i]->SetTileIndex(FIntPoint(i % LevelGenerationInfo.TileCount.X, i / LevelGenerationInfo.TileCount.X));
		currentX += TileDimensions.X;
	}
}

ATile* ULevelGenerator::GetTileFromIndex(int32 X, int32 Y)
{
	ensure(X < LevelGenerationInfo.TileCount.X && Y < LevelGenerationInfo.TileCount.Y);

	return Tiles[LevelGenerationInfo.TileCount.X * Y + X];
}

void ULevelGenerator::DestroyLevel()
{
	if (!bRunning)
	{
		UE_LOG(LevelGenerator, Warning, TEXT("Trying to destroy a level that's already destroyed"))
		return;
	}

	for (auto tile : Tiles)
	{
		if (IsValid(tile))
		{
			tile->Destroy();
		}
	}
	Tiles.Empty();
	bRunning = false;
}