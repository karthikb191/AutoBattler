// Fill out your copyright notice in the Description page of Project Settings.


#include "SubSystems/BattleSubsystem.h"
#include "SubSystems/Game/GameSubsystem.h"
#include "Math/IntPoint.h"
#include "Level/LevelGenerator.h"
#include "Level/Tile.h"
#include "Character/Creature.h"
#include "Synchronizer.h"

void UBattleSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	//GameSubsystem = Collection.InitializeDependency<UGameSubsystem>();
	//check(GameSubsystem != nullptr)
}

void UBattleSubsystem::InitLevel(const FBattleInfo& Info)
{
	BattleInfo = Info;
}

void UBattleSubsystem::MakeBattlePreparations()
{
	GameSubsystem = GetWorld()->GetGameInstance()->GetSubsystem<UGameSubsystem>();
	check(GameSubsystem != nullptr);
	SpawnCreatures();

	TDelegate<void(uint32)> del;
	del.BindUObject(this, &UBattleSubsystem::Synchronize);
	GameSubsystem->GetSynchronizer()->GetTickCallback().Add(del);
}

void UBattleSubsystem::SpawnCreatures()
{
	FActorSpawnParameters spawnParams;
	spawnParams.bNoFail = true;
	spawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	
	const FLevelGenerationInfo& levelInfo = GameSubsystem->GetLevelGenerator()->GetLevelInfo();
	const TArray<ATile*>& tiles = GameSubsystem->GetLevelGenerator()->GetTiles();

	uint32 randIndex = FMath::RandRange(0, tiles.Num());
	FVector spawnPoint(tiles[randIndex]->GetActorLocation());
	
	CreatureA = (ACreature*)GetWorld()->SpawnActor<ACreature>(BattleInfo.CreatureA.Get(),
		FTransform(FQuat::Identity, spawnPoint, FVector::One()), spawnParams);
	CreatureA->SetCurrentTile(tiles[randIndex]);

	randIndex = FMath::RandRange(0, tiles.Num());
	spawnPoint = tiles[randIndex]->GetActorLocation();
	CreatureB = (ACreature*)GetWorld()->SpawnActor<ACreature>(BattleInfo.CreatureB.Get(),
		FTransform(FQuat::Identity, spawnPoint, FVector::One()), spawnParams);
	CreatureB->SetCurrentTile(tiles[randIndex]);
}

void UBattleSubsystem::Synchronize(uint32 TimeStep)
{
	ACreature* Target = GetClosestEnemyTo(CreatureA);
	TArray<ATile*> Path = TraceCreaturePath(CreatureA, Target);
	CreatureA->SetTilesToTraverse(Path);
	for (auto Tile : Path)
	{
		DrawDebugSphere(GetWorld(), Tile->GetActorLocation(), 20.0f, 10, FColor::Blue, false, .1f);
	}


	Target = GetClosestEnemyTo(CreatureB);
	Path = TraceCreaturePath(CreatureB, Target);
	CreatureB->SetTilesToTraverse(Path);
	for (auto Tile : Path)
	{
		DrawDebugSphere(GetWorld(), Tile->GetActorLocation(), 25.0f, 10, FColor::Red, false, .1f);
	}
}

//Movement
ACreature* UBattleSubsystem::GetClosestEnemyTo(ACreature* Searcher)
{
	//TODO: Dirty Hack for now. Remove this one multiple enemies are present
	if (Searcher == CreatureA) return CreatureB;
	else return CreatureA;
}

TArray<ATile*> UBattleSubsystem::TraceCreaturePath(ACreature* From, ACreature* To)
{
	//Uses DDA line drawing to trace as there are no blockers
	TArray<ATile*> traversePath;
	FIntPoint Start = From->GetCurrentTile()->GetTileIndex();
	FIntPoint End = To->GetCurrentTile()->GetTileIndex();
	
	if (Start == End) return traversePath;
	
	FVector2D Current = Start;
	int dx = End.X - Start.X;
	int dy = End.Y - Start.Y;
	
	int numItr = FMath::Abs(dx) > FMath::Abs(dy) ? FMath::Abs(dx) : FMath::Abs(dy);
	float xInc = (float)dx / (float)numItr;
	float yInc = (float)dy / (float)numItr;

	for(int i = 0; i < numItr - 1; i++)
	{
		Current.X = Current.X + xInc;
		Current.Y = Current.Y + yInc;

		ATile* Tile = GameSubsystem->GetLevelGenerator()->GetTileFromIndex(FMath::Floor(Current.X), FMath::Floor(Current.Y));
		traversePath.Push(Tile);
	}

	return traversePath;
}

//Battle
void UBattleSubsystem::Attack()
{

}