// Fill out your copyright notice in the Description page of Project Settings.


#include "SpawnLayoutActor.h"

#include "TileAssetSpawnComponent.h"


ASpawnLayoutActor::ASpawnLayoutActor()
{
	PrimaryActorTick.bCanEverTick = false;
	
	Hexagon = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Hexagon"));
	RootComponent = Hexagon;
}

FSpawnLayoutStruct ASpawnLayoutActor::GetSpawnLayoutStruct()
{
	FSpawnLayoutStruct LayoutStruct = FSpawnLayoutStruct();
	LayoutStruct.Name = FName(GetName());
	LayoutStruct.GameplayTagRules = GameplayTagRules;
	LayoutStruct.GuaranteedIfPossible = GuaranteedIfPossible;
	LayoutStruct.SpawnBias = SpawnBias;

	for (UActorComponent* Component : GetComponents())
	{
		UTileAssetSpawnComponent* SpawnComponent = Cast<UTileAssetSpawnComponent>(Component);
		if (SpawnComponent)
		{
			LayoutStruct.SpawnLayout.AddSpawnPoint(
				SpawnComponent->GetTileAssetCategory(),
				SpawnComponent->GetSpawnPoint());
		}
	}
	
	return LayoutStruct;
}
