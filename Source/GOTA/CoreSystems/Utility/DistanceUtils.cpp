// Fill out your copyright notice in the Description page of Project Settings.


#include "DistanceUtils.h"

#include "GOTA/TileMap/HexCoordsFunctions.h"
#include "GOTA/TileMap/TileMap.h"

const float ADistanceUtils::ActiveTileRange = 4;

void ADistanceUtils::BeginPlay()
{
	Super::BeginPlay();
	CachedGameState = GetWorld()->GetGameState<AGOTAGameState>();
	LastCoords = FHexCoords(-100,-100);
}

void ADistanceUtils::UpdateDistanceToTiles()
{
	if(!CachedGameState->TileMap) return;
	
	FVector CurrentLocation = GetActorLocation();
	FHexCoords CurrentCoords = UHexCoordsFunctions::VectorToHexCoords(CurrentLocation);
	if(LastCoords != CurrentCoords)
	{
		TArray<FHexCoords> CurrentCoordsInRange = UHexCoordsFunctions::GetAllCoordsInRange(CurrentCoords, ActiveTileRange);

		for(FHexCoords Coords : CurrentCoordsInRange)
		{
			if(!LastCoordsInRange.Contains(Coords))
			{
				if(ATile* Tile = CachedGameState->TileMap->GetTile(Coords))
				{
					Tile->OnEnteringActiveRangeOfGuardian();
				}
			}
		}

		for(FHexCoords Coords : LastCoordsInRange)
		{
			if(!CurrentCoordsInRange.Contains(Coords))
			{
				if(ATile* Tile = CachedGameState->TileMap->GetTile(Coords))
				{
					Tile->OnLeavingActiveRangeOfGuardian();
				}
			}
		}
		
		LastCoordsInRange = CurrentCoordsInRange;
		LastCoords = CurrentCoords;
	}
}