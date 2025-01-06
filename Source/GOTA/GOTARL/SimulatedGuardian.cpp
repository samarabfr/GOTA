// Fill out your copyright notice in the Description page of Project Settings.


#include "SimulatedGuardian.h"

#include "GOTA/CoreSystems/Faction/Settlement/Colony.h"
#include "GOTA/CoreSystems/GameplayFramework/GS_Ingame.h"
#include "GOTA/CoreSystems/Tile/TileMap.h"

void ASimulatedGuardian::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	if (bWantsToMove && !MoveDirection.IsZero())
	{
		AddMovementInput(FVector(MoveDirection.X, MoveDirection.Y, 0.0f));
	}
	if (!TargetTile.IsValid() &&
		GameState.IsValid() &&
		GameState->GetColony() &&
		GameState->GetColony()->ClaimedTiles.Num() > 0)
	{
		TargetTile = GameState->GetColony()->ClaimedTiles[0];
	}
}

void ASimulatedGuardian::BeginPlay()
{
	Super::BeginPlay();
	GameState = Cast<AGS_Ingame>(GetWorld()->GetGameState());
}

void ASimulatedGuardian::ResetToRandomTile()
{
	if (!TargetTile.IsValid() &&
		GameState.IsValid() &&
		GameState->GetTileMap())
	{
		const ATile* RandomTile = GameState->GetTileMap()->GetRandomTile();
		TeleportTo(RandomTile->GetActorTransform().GetLocation(),
		           RandomTile->GetActorTransform().GetRotation().Rotator());
	}
}
