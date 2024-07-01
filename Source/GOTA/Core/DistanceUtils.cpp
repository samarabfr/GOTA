// Fill out your copyright notice in the Description page of Project Settings.


#include "DistanceUtils.h"

#include "GOTA/TileMap/HexCoordsFunctions.h"
#include "GOTA/TileMap/TileMap.h"

const float ADistanceUtils::ActiveTileRange = 4;

ADistanceUtils::ADistanceUtils()
{
	// Create a root scene component and set it as the root component
	SetRootComponent(CreateDefaultSubobject<USceneComponent>(TEXT("RootSceneComponent")));

	// Enable ticking for this actor
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;
	PrimaryActorTick.bTickEvenWhenPaused = true;
	PrimaryActorTick.SetTickFunctionEnable(true);
	
	// Ensure this actor can be loaded on clients
	bNetLoadOnClient = true;
}

void ADistanceUtils::BeginPlay()
{
	Super::BeginPlay();
	CachedGameState = GetWorld()->GetGameState<AGOTAGameState>();
	LastCoords = FHexCoords(-100,-100);
	
	// Check if actor is hidden
	if (IsHidden())
	{
		UE_LOG(LogTemp, Error, TEXT("Actor is hidden"));
	}

	// Check if ticking is enabled
	if (!PrimaryActorTick.IsTickFunctionEnabled())
	{
		UE_LOG(LogTemp, Error, TEXT("Tick function is not enabled"));
	}

	UE_LOG(LogTemp, Warning, TEXT("Role: %d"), GetLocalRole());
	UE_LOG(LogTemp, Warning, TEXT("NetMode: %d"), GetNetMode());
}

void ADistanceUtils::Tick(float DeltaSeconds)
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
