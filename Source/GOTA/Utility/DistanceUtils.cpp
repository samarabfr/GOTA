// Fill out your copyright notice in the Description page of Project Settings.


#include "DistanceUtils.h"


#include "DistanceUtilsSettings.h"
#include "GOTA/GameplayFramework/GS_Ingame.h"
#include "GOTA/Tilemap/HexCoordsFunctions.h"
#include "GOTA/Tilemap/TileMap.h"

ADistanceUtils::ADistanceUtils()
{
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;
	PrimaryActorTick.TickInterval = 0.2f;
	
	RootComponent = CreateDefaultSubobject<USceneComponent>("ROOT");
}

void ADistanceUtils::BeginPlay()
{
	Super::BeginPlay();
	GameState = GetWorld()->GetGameState<AGS_Ingame>();
	CurrentCoords = FHexCoords(-100, -100);
}

void ADistanceUtils::Tick(float DeltaSeconds)
{
	// TileMap doesn't exist yet. Idk why this should ever happen but it did. LoadingManager should prevent this
	if (!GameState->GetTileMap()) return;
	FVector CurrentLocation = GetActorLocation();
	FHexCoords NewCoords = UHexCoordsFunctions::VectorToHexCoords(CurrentLocation);
	if (CurrentCoords == NewCoords) return;
	// We are on a New Tile
	CurrentCoords = NewCoords;
	// UpdateDistanceToTiles();
	//UpdateDistanceToCombats();
}

void ADistanceUtils::UpdateDistanceToTiles()
{
	TArray<FHexCoords> NextCoordsInRange
		= UHexCoordsFunctions::GetAllCoordsInRange(CurrentCoords, Settings->ActiveTileRangeInTiles);
	for (FHexCoords Coords : NextCoordsInRange)
	{
		if (!CurrentCoordsInRange.Contains(Coords))
		{
			if (ATile* Tile = GameState->GetTileMap()->GetTile(Coords))
			{
				// Tile->OnEnteringActiveRangeOfGuardian();
			}
		}
	}
	for (FHexCoords Coords : CurrentCoordsInRange)
	{
		if (!NextCoordsInRange.Contains(Coords))
		{
			if (ATile* Tile = GameState->GetTileMap()->GetTile(Coords))
			{
				// Tile->OnLeavingActiveRangeOfGuardian();
			}
		}
	}
	CurrentCoordsInRange = NextCoordsInRange;
}
