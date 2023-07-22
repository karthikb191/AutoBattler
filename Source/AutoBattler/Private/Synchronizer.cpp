// Fill out your copyright notice in the Description page of Project Settings.


#include "Synchronizer.h"

// Sets default values
ASynchronizer::ASynchronizer()
{
 	PrimaryActorTick.bCanEverTick = true;
	SetActorTickEnabled(false);
}

void ASynchronizer::Reset()
{
	CurrentTimeStep = 0;
	SetActorTickEnabled(false);
	TickCallback.Clear();
}

void ASynchronizer::Start()
{
	if (bRunning) return;

	SetActorTickEnabled(true);
	SetActorTickInterval(0.1f);
	
	//TODO: Hardcoding tick interval for now. Get it from a config file or data asset there are multiple global settings
	bRunning = true;
}

void ASynchronizer::Stop()
{
	if (!bRunning) return;

	bRunning = false;
	Reset();
}

// Called when the game starts or when spawned
void ASynchronizer::BeginPlay()
{
	Super::BeginPlay();
	SetActorTickEnabled(false);
}

// Called every frame
void ASynchronizer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	++CurrentTimeStep;
	TickCallback.Broadcast(CurrentTimeStep);
}

