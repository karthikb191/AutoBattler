// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Synchronizer.generated.h"

DECLARE_MULTICAST_DELEGATE_OneParam(SynchronizerTickDelagate, const uint32_t);

UCLASS()
class AUTOBATTLER_API ASynchronizer : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASynchronizer();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	void Start();
	void Stop();
	void Reset();

	SynchronizerTickDelagate& GetTickCallback() { return TickCallback; }
	uint32_t GetCurrentTimeStep() { return CurrentTimeStep; }

	// Called every frame
	virtual void Tick(float DeltaTime) override;


private:
	SynchronizerTickDelagate		TickCallback;

	uint32_t						CurrentTimeStep = 0;
	bool							bRunning = false;
};
