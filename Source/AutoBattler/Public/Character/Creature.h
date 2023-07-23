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
	void SetTilesToTraverse(const TArray<ATile*>& Tiles);

	void Move();
	void Stop();
	void Move_Visualize(float DeltaTime);

	/*After initiating Hit, creature cant do anything till it's action frames have expires*/
	void Hit(uint32 TimeStamp);
	void Hit_Visualize();

	void Die(uint32 TimeStamp);

	void TakeAHit(float Damage);
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UCreatureStatsComponent* GetCreatureStatsComponent() { return Stats; }
	uint32 GetLastHitCommandTimestamp() { return LastHitCommand; }
	bool GetIsMarkedForDeath() { return bMarkForDeath; }
private:
	void Hit_Internal();

	UPROPERTY(EditDefaultsOnly)
	USceneComponent*			MeshRoot;
	UPROPERTY(EditDefaultsOnly)
	UStaticMeshComponent*		Mesh;
	UPROPERTY(EditDefaultsOnly)
	UCreatureStatsComponent*	Stats;
	
	FLinearColor				DefColor;
	FLinearColor				DamageColorModifier	= FLinearColor::White;
	UMaterialInstanceDynamic*	MeshMaterial;

	
	float						TotalTravel = 0.0f;
	float						TotalTravelBeforeMove = 0.0f;
	float						SpeedRequired = 0.0f;
	//int							TileIndexInListAfterMoveCmd = -1;

	TArray<ATile*>				TilesToTraverse;
	UPROPERTY(Transient)
	ATile*						CurrentTile;

	uint32						LastHitCommand = 0;
	bool						bMarkForDeath = false;
};
