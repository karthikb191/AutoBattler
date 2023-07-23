// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Creature.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SceneComponent.h"
#include "Level/Tile.h"
#include "Character/CreatureStatsComponent.h"

// Sets default values

ACreature::ACreature()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	SetActorTickEnabled(true);
	MeshRoot = CreateDefaultSubobject<USceneComponent>(TEXT("MeshRoot"));
	MeshRoot->SetupAttachment(RootComponent);
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(MeshRoot);
	Stats = CreateDefaultSubobject<UCreatureStatsComponent>(TEXT("Stats"));
}

// Called when the game starts or when spawned
void ACreature::BeginPlay()
{
	Super::BeginPlay();

	MeshMaterial = UMaterialInstanceDynamic::Create(Mesh->GetMaterial(0), Mesh);
	MeshMaterial->GetVectorParameterValue(FName("Color"), DefColor);
	Mesh->SetMaterial(0, MeshMaterial);
}

// Called every frame
void ACreature::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	Move_Visualize(DeltaTime);
	Hit_Visualize();
}

void ACreature::SetTilesToTraverse(const TArray<ATile*>& Tiles)
{
	bool bAddNewTiles = false;
	int NumTiles = FMath::Min(TilesToTraverse.Num(), Tiles.Num());
	for (int i = 0; i < NumTiles; i++)
	{
		if (TilesToTraverse[i] != Tiles[i])
		{
			bAddNewTiles = true;
			break;
		}
	}
	
	if (TilesToTraverse.Num() != Tiles.Num())
	{
		bAddNewTiles = true;
	}
	
	if (bAddNewTiles)
	{
		//If out initial tile didnt change from previous time step, don't reset Travel params
		if (TilesToTraverse.Num() > 1 && Tiles.Num() > 1)
		{
			if (TilesToTraverse[0] != Tiles[0])
			{
				TotalTravel = 0.f;
				TotalTravelBeforeMove = 0.f;
			}
		}
		else
		{
			TotalTravel = 0.f;
			TotalTravelBeforeMove = 0.f;
		}
		TilesToTraverse = Tiles;
	}
}

void ACreature::Move()
{
	if (TilesToTraverse.Num() == 0) return;

	int32 prevTraversal = FMath::Floor(TotalTravel);
	TotalTravelBeforeMove = TotalTravel;
	TotalTravel += GetCreatureStatsComponent()->GetTilesTraversedPerTimeStamp();
	
	SpeedRequired = (TotalTravel - TotalTravelBeforeMove) / 0.1f; //TODO: Replace with timestamp interval
	
	int32 postTraverasal = FMath::Floor(TotalTravel);
	if (postTraverasal > prevTraversal)
	{
		if (postTraverasal >= TilesToTraverse.Num() - 1)
		{
			CurrentTile = TilesToTraverse[TilesToTraverse.Num() - 1];
		}
		else
		{
			CurrentTile = TilesToTraverse[postTraverasal];
		}
	}
}

void ACreature::Move_Visualize(float DeltaTime)
{
	if (TotalTravelBeforeMove >= TotalTravel) return;

	TotalTravelBeforeMove += SpeedRequired * DeltaTime;
	if (TotalTravelBeforeMove >= TotalTravel)
	{
		return;
	}

	int PrevTileIndex = FMath::Floor(TotalTravelBeforeMove);
	int NextTileIndex = FMath::CeilToInt(TotalTravelBeforeMove);

	if (NextTileIndex >= TilesToTraverse.Num())
	{
		SetActorLocation(CurrentTile->GetActorLocation());
		return;
	}

	float lerpFactor = TotalTravelBeforeMove < 1.f ? 
		TotalTravelBeforeMove : 
		FMath::Fmod(TotalTravelBeforeMove, FMath::Floor(TotalTravelBeforeMove));

	FVector resLocation = FMath::Lerp(TilesToTraverse[PrevTileIndex]->GetActorLocation(),
		TilesToTraverse[NextTileIndex]->GetActorLocation(), lerpFactor);

	SetActorLocation(resLocation);
}

void ACreature::Hit(uint32 TimeStamp)
{
	LastHitCommand = TimeStamp;
	GEngine->AddOnScreenDebugMessage(-1, 5.f, 
		FColor::Green, 
		FString::Printf(TEXT("%s dealt %f damage"), *GetActorLabel(), Stats->GetDamagePerHit()));
}

void ACreature::Stop()
{
	SetActorLocation(CurrentTile->GetActorLocation());
	TotalTravel = 0.f;
	TotalTravelBeforeMove = 0.f;
	TilesToTraverse.Empty();
}

void ACreature::Hit_Visualize()
{
	DamageColorModifier = FMath::Lerp(DamageColorModifier, FLinearColor::White, 0.05f);
	MeshMaterial->SetVectorParameterValue("Color", DamageColorModifier * DefColor);
}

void ACreature::Die(uint32 TimeStamp)
{
	bMarkForDeath = true;
	//TODO: Add some kind of animation to signal death
	GEngine->AddOnScreenDebugMessage(-1, 5.f,
		FColor::Red,
		FString::Printf(TEXT("%s is Dead!"), *GetActorLabel()));
	Destroy();
}

void ACreature::TakeAHit(float Damage)
{
	Stats->TakeDamage(Damage);
	GEngine->AddOnScreenDebugMessage(-1, 5.f,
		FColor::Yellow,
		FString::Printf(TEXT("%s took %f damage. Remaining: %f"), *GetActorLabel(), Damage, Stats->GetHitPointsRemaining()));

	DamageColorModifier = FLinearColor::Black;
}
