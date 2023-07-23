// Fill out your copyright notice in the Description page of Project Settings.


#include "SubSystems/BattleSubsystem.h"
#include "SubSystems/Game/GameSubsystem.h"
#include "Math/IntPoint.h"
#include "Level/LevelGenerator.h"
#include "Level/Tile.h"
#include "Character/Creature.h"
#include "Character/CreatureStatsComponent.h"
#include "Synchronizer.h"

//Helper Functions
void RunBattleLogicOn(UBattleSubsystem* BattleSystem, ACreature* Creature, uint32 TimeStep);

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

void UBattleSubsystem::Synchronize(uint32 TimeStep)
{
	RunBattleLogicOn(this, CreatureA, TimeStep);
	RunBattleLogicOn(this, CreatureB, TimeStep);
}

void UBattleSubsystem::SpawnCreatures()
{
	FActorSpawnParameters spawnParams;
	spawnParams.bNoFail = true;
	spawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	
	const FLevelGenerationInfo& levelInfo = GameSubsystem->GetLevelGenerator()->GetLevelInfo();
	const TArray<ATile*>& tiles = GameSubsystem->GetLevelGenerator()->GetTiles();

	uint32 randIndex = FMath::RandRange(0, tiles.Num() - 1);
	FVector spawnPoint(tiles[randIndex]->GetActorLocation());
	
	CreatureA = (ACreature*)GetWorld()->SpawnActor<ACreature>(BattleInfo.CreatureA.Get(),
		FTransform(FQuat::Identity, spawnPoint, FVector::One()), spawnParams);
	CreatureA->SetCurrentTile(tiles[randIndex]);

	randIndex = FMath::RandRange(0, tiles.Num() - 1);
	spawnPoint = tiles[randIndex]->GetActorLocation();
	CreatureB = (ACreature*)GetWorld()->SpawnActor<ACreature>(BattleInfo.CreatureB.Get(),
		FTransform(FQuat::Identity, spawnPoint, FVector::One()), spawnParams);
	CreatureB->SetCurrentTile(tiles[randIndex]);
}

//Movement
ACreature* UBattleSubsystem::GetClosestEnemyTo(ACreature* Searcher)
{
	//TODO: Dirty Hack for now. Remove this one multiple enemies are present
	if (!IsValid(Searcher)) return nullptr;

	if (Searcher == CreatureA) {
		if (IsValid(CreatureB) && !CreatureB->GetIsMarkedForDeath())
		{
			return CreatureB;
		}
		return nullptr;
	}

	if (IsValid(CreatureA) && !CreatureA->GetIsMarkedForDeath())
	{
		return CreatureA;
	}
	return nullptr;
}

TArray<ATile*> UBattleSubsystem::TraceCreaturePath(ACreature* From, ACreature* To)
{
	//Uses DDA line drawing to trace as there are no blockers
	TArray<ATile*> traversePath;
	FIntPoint Start = From->GetCurrentTile()->GetTileIndex();
	FIntPoint End = To->GetCurrentTile()->GetTileIndex();
	
	if (Start == End) return traversePath;

	traversePath.Push(From->GetCurrentTile());
	
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

//Helpers
void RunBattleLogicOn(UBattleSubsystem* BattleSystem, ACreature* Creature, uint32 TimeStep)
{
	if (!IsValid(Creature)) return;
	ACreature* Target = BattleSystem->GetClosestEnemyTo(Creature);
	if (!IsValid(Target)) return;

	TArray<ATile*> Path = BattleSystem->TraceCreaturePath(Creature, Target);

	float Range = Creature->GetCreatureStatsComponent()->GetAttackRange();
	float HitDuration = Creature->GetCreatureStatsComponent()->GetAttackDuration();
	float CooldownDuration = Creature->GetCreatureStatsComponent()->GetCooldownDuration();
	uint32 LastHitCommand = Creature->GetLastHitCommandTimestamp();
	if (LastHitCommand + HitDuration < TimeStep)
	{
		if (Path.Num() <= Range)
		{
			//If we are past attack and cool down frames, initiate one more hit on target
			if (LastHitCommand + HitDuration + CooldownDuration < TimeStep)
			{
				Creature->Hit(TimeStep);
				Target->TakeAHit(Creature->GetCreatureStatsComponent()->GetDamagePerHit());

				if (Target->GetCreatureStatsComponent()->GetHitPointsRemaining() <= 0)
				{
					Target->Die(TimeStep);
				}
			}
		}
		else
		{
			Creature->SetTilesToTraverse(Path);
			for (auto Tile : Path)
			{
				DrawDebugSphere(Creature->GetWorld(), Tile->GetActorLocation(), 20.0f, 10, FColor::Blue, false, .15f);
			}
			Creature->Move();
		}
	}
}