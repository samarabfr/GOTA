// Fill out your copyright notice in the Description page of Project Settings.


#include "SpawnLayoutActor.h"


ASpawnLayoutActor::ASpawnLayoutActor()
{
	PrimaryActorTick.bCanEverTick = false;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("ROOT"));

	Hexagon = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Hexagon"));
	Hexagon->SetupAttachment(RootComponent);
}

FSpawnLayoutStruct ASpawnLayoutActor::GetSpawnLayout()
{
	FSpawnLayoutStruct SpawnLayout = FSpawnLayoutStruct();
	SpawnLayout.Name = FName(GetName());
	SpawnLayout.GameplayTagRules = GameplayTagRules;
	SpawnLayout.GuaranteedIfPossible = GuaranteedIfPossible;
	SpawnLayout.SpawnBias = SpawnBias;
	return SpawnLayout;
}
