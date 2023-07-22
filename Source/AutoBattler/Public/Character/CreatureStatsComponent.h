// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CreatureStatsComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class AUTOBATTLER_API UCreatureStatsComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UCreatureStatsComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void TakeDamage(float Damage)
	{
		HitPoints -= Damage;
	}
	
	float GetHitPointsRemaining() { return HitPoints; }
	float GetTilesTraversedPerTimeStamp() { return TilesTraversedPerTimeStamp; }
	float GetAttackDuration() { return AttackDuration; }
	float GetCooldownDuration() { return CooldownDuration; }
	float GetDamagePerHit() { return DamagePerHit; }
	float GetAttackRange() { return AttackRange; }

protected:
	UPROPERTY(EditAnywhere)
	float						HitPoints = 50.0f;

	UPROPERTY(EditAnywhere)
	float						TilesTraversedPerTimeStamp = 0.2f;

	/*All of the following values are in terms of time slices*/
	UPROPERTY(EditAnywhere)
	float						AttackDuration = 5.0f;
	UPROPERTY(EditAnywhere)
	float						CooldownDuration = 2.0f;
	UPROPERTY(EditAnywhere)
	float						DamagePerHit = 10.0f;
	UPROPERTY(EditAnywhere)
	float						AttackRange = 1.0f;
};
