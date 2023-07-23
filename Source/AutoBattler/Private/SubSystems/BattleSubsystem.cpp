// Fill out your copyright notice in the Description page of Project Settings.


#include "SubSystems/BattleSubsystem.h"
#include "SubSystems/Game/GameSubsystem.h"
#include "Math/IntPoint.h"
#include "Level/LevelGenerator.h"
#include "Level/Tile.h"
#include "Character/Creature.h"
#include "Character/CreatureStatsComponent.h"
#include "Synchronizer.h"

FColor DebugSphereColor = FColor::Red;
//Helper Functions
void RunBattleLogicOn(UBattleSubsystem* BattleSystem, ACreature* Creature, TArray<ACreature*>& OppositeTeam, uint32 TimeStep);
void PopulateTeam(TArray<ACreature*>& Team, const TArray<TSubclassOf<ACreature>>& CreatureClasses,
	UBattleSubsystem* BattleSystem, UGameSubsystem* GameSubsystem);
ACreature* GetClosestEnemyTo(ACreature* Searcher, TArray<ACreature*>& OppositeTeam);

DECLARE_LOG_CATEGORY_EXTERN(BattleSubsystemLogger, Warning, Warning);
DEFINE_LOG_CATEGORY(BattleSubsystemLogger)

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

	bBattleRunning = true;
}

void UBattleSubsystem::PostBattleCleanup()
{
	BattleFinishedCallback.Clear();
	for (ACreature* Creature : TeamA)
	{
		if (IsValid(Creature))
		{
			Creature->Destroy();
		}
	}
	for (ACreature* Creature : TeamB)
	{
		if (IsValid(Creature))
		{
			Creature->Destroy();
		}
	}
	TeamA.Empty();
	TeamB.Empty();
}

void UBattleSubsystem::Synchronize(uint32 TimeStep)
{
	if (!bBattleRunning) return;

	if (TeamA.IsEmpty() || TeamB.IsEmpty())
	{
		if (BattleFinishedCallback.IsBound())
		{
			BattleFinishedCallback.Broadcast();
			bBattleRunning = false;
			return;
		}
	}

	DebugSphereColor = FColor::Red;
	for (ACreature* Creature : TeamA)
	{
		RunBattleLogicOn(this, Creature, TeamB, TimeStep);
	}
	DebugSphereColor = FColor::Blue;
	for (ACreature* Creature : TeamB)
	{
		RunBattleLogicOn(this, Creature, TeamA, TimeStep);
	}
}

void UBattleSubsystem::SpawnCreatures()
{
	PopulateTeam(TeamA, BattleInfo.TeamA, this, GameSubsystem);
	PopulateTeam(TeamB, BattleInfo.TeamB, this, GameSubsystem);
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

	float epsilon = 0.000001f;
	for(int i = 0; i < numItr; i++)
	{
		Current.X = Current.X + xInc + epsilon;
		Current.Y = Current.Y + yInc + epsilon;

		ATile* Tile = GameSubsystem->GetLevelGenerator()->GetTileFromIndex(FMath::Floor(Current.X), FMath::Floor(Current.Y));
		//if(Tile->GetTileIndex() != End)
		traversePath.Push(Tile);
	}

	return traversePath;
}

//Helpers
void RunBattleLogicOn(UBattleSubsystem* BattleSystem, ACreature* Creature, TArray<ACreature*>& OppositeTeam, uint32 TimeStep)
{
	if (!IsValid(Creature)) return;
	ACreature* Target = GetClosestEnemyTo(Creature, OppositeTeam);
	if (!IsValid(Target)) return;

	TArray<ATile*> Path = BattleSystem->TraceCreaturePath(Creature, Target);

	float Range = Creature->GetCreatureStatsComponent()->GetAttackRange();
	float HitDuration = Creature->GetCreatureStatsComponent()->GetAttackDuration();
	float CooldownDuration = Creature->GetCreatureStatsComponent()->GetCooldownDuration();
	uint32 LastHitCommand = Creature->GetLastHitCommandTimestamp();
	if (LastHitCommand + HitDuration < TimeStep)
	{
		if (Path.Num() - 1 <= Range)
		{
			//If we are past attack and cool down frames, initiate one more hit on target
			if (LastHitCommand + HitDuration + CooldownDuration < TimeStep)
			{
				Creature->Stop();
				Creature->Hit(TimeStep);
				Target->TakeAHit(Creature->GetCreatureStatsComponent()->GetDamagePerHit());

				if (Target->GetCreatureStatsComponent()->GetHitPointsRemaining() <= 0)
				{
					OppositeTeam.Remove(Target);
					Target->Die(TimeStep);
				}
			}
		}
		else
		{
			Creature->SetTilesToTraverse(Path);
			for (auto Tile : Path)
			{
				DrawDebugSphere(Creature->GetWorld(), Tile->GetActorLocation(), 20.0f, 10, DebugSphereColor, false, .15f);
			}
			Creature->Move();
		}
	}
}

void PopulateTeam(TArray<ACreature*>& Team, const TArray<TSubclassOf<ACreature>>& CreatureClasses,
	UBattleSubsystem* BattleSystem, UGameSubsystem* GameSubsystem)
{
	FActorSpawnParameters spawnParams;
	spawnParams.bNoFail = true;
	spawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	const FLevelGenerationInfo& levelInfo = GameSubsystem->GetLevelGenerator()->GetLevelInfo();
	const TArray<ATile*>& tiles = GameSubsystem->GetLevelGenerator()->GetTiles();

	uint32 randIndex = FMath::RandRange(0, tiles.Num() - 1);
	FVector spawnPoint(tiles[randIndex]->GetActorLocation());

	TArray<ATile*> OccupiedTiles;
	for (TSubclassOf<ACreature> CreatureClass : CreatureClasses)
	{
		ACreature* Creature = (ACreature*)BattleSystem->GetWorld()->SpawnActor<ACreature>(CreatureClass.Get(),
			FTransform(FQuat::Identity, spawnPoint, FVector::One()), spawnParams);
		ATile* Tile = tiles[randIndex];

		int retries = 5;
		while (OccupiedTiles.Contains(Tile) && retries > 0)
		{
			Tile = tiles[FMath::RandRange(0, tiles.Num() - 1)];
			--retries;
		}

		if (!OccupiedTiles.Contains(Tile))
		{
			Creature->SetCurrentTile(Tile);
			Team.Push(Creature);
			OccupiedTiles.Push(Tile);
		}
		else
		{
			UE_LOG(BattleSubsystemLogger, Warning, TEXT("Could not spawn a creature. There may not be enough free tiles"));
		}
	}
}

ACreature* GetClosestEnemyTo(ACreature* Searcher, TArray<ACreature*>& OppositeTeam)
{
	if (!IsValid(Searcher) || OppositeTeam.IsEmpty()) return nullptr;

	FIntPoint Start = Searcher->GetCurrentTile()->GetTileIndex();
	uint32 sqrDist = TNumericLimits<uint32>::Max();
	ACreature* Target = nullptr;
	for (ACreature* Creature : OppositeTeam)
	{
		FIntPoint End = Searcher->GetCurrentTile()->GetTileIndex();
		float dx = End.X - Start.X;
		float dy = End.Y - Start.Y;
		float curSqrDist = (dx * dx + dy * dy);
		if (curSqrDist < sqrDist)
		{
			Target = Creature;
			sqrDist = curSqrDist;
		}
	}

	return Target;
}