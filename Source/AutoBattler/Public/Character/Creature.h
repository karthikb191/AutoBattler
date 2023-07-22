// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Creature.generated.h"

class UStaticMeshComponent;
class USceneComponent;
class UCreatureStatsComponent;
class ATile;

UCLASS()
class AUTOBATTLER_API ACreature : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACreature();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	void SetCurrentTile(ATile* Tile) { CurrentTile = Tile; }
	ATile* GetCurrentTile() { return CurrentTile; }
	void SetTilesToTraverse(const TArray<ATile*>& Tiles) { TilesToTraverse = Tiles; }

	void Move();
	void Move_Visualize();

	void Hit();
	void Hit_Visualize();
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	UPROPERTY(EditDefaultsOnly)
	USceneComponent*			MeshRoot;
	UPROPERTY(EditDefaultsOnly)
	UStaticMeshComponent*		Mesh;
	UPROPERTY(EditDefaultsOnly)
	UCreatureStatsComponent*	Stats;
	
	//TODO: Move this to stats
	UPROPERTY(EditAnywhere)
	float						tilesPerTimeStamp = 1.0f;
	float						tilesTraversed = 0.0f;

	TArray<ATile*>				TilesToTraverse;
	UPROPERTY(Transient)
	ATile*						CurrentTile;
};
