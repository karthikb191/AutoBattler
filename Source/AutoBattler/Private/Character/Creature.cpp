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
	MeshRoot = CreateDefaultSubobject<USceneComponent>(TEXT("MeshRoot"));
	MeshRoot->SetupAttachment(RootComponent);
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(MeshRoot);
	Stats = CreateDefaultSubobject<UCreatureStatsComponent>(TEXT("Stats"));
	//Stats->SetupAttachment(MeshRoot);
}

// Called when the game starts or when spawned
void ACreature::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ACreature::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ACreature::Move()
{
	if (TilesToTraverse.Num() == 0) return;

	int32 prevTraversal = FMath::Floor(tilesTraversed);
	tilesTraversed += tilesPerTimeStamp;
	
	int32 postTraverasal = FMath::Floor(tilesTraversed);
	if (postTraverasal > prevTraversal)
	{
		if (postTraverasal > TilesToTraverse.Num())
		{
			CurrentTile = TilesToTraverse[TilesToTraverse.Num() - 1];
		}
		else
		{
			CurrentTile = TilesToTraverse[postTraverasal];
		}
	}

	//TODO: Remove this once move visualization logic is in place
	SetActorLocation(CurrentTile->GetActorLocation());
}

void ACreature::Move_Visualize()
{

}

void ACreature::Hit()
{

}
void ACreature::Hit_Visualize()
{

}
