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
	//Stats->SetupAttachment(MeshRoot);
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

	Hit_Visualize();
}

void ACreature::Move()
{
	if (TilesToTraverse.Num() == 0) return;

	int32 prevTraversal = FMath::Floor(tilesTraversed);
	tilesTraversed += GetCreatureStatsComponent()->GetTilesTraversedPerTimeStamp();
	
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

void ACreature::Hit(uint32 TimeStamp)
{
	LastHitCommand = TimeStamp;
	GEngine->AddOnScreenDebugMessage(-1, 5.f, 
		FColor::Green, 
		FString::Printf(TEXT("%s dealt %f damage"), *GetActorLabel(), Stats->GetDamagePerHit()));
}
void ACreature::Hit_Visualize()
{
	DamageColorModifier = FMath::Lerp(DamageColorModifier, FLinearColor::White, 0.1f);
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
