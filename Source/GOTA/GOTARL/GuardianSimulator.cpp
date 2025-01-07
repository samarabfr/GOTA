// Fill out your copyright notice in the Description page of Project Settings.


#include "GuardianSimulator.h"

#include "GOTA/CoreSystems/Faction/Settlement/Colony.h"
#include "GOTA/CoreSystems/GameplayFramework/GS_Ingame.h"
#include "GOTA/CoreSystems/Tile/TileMap.h"

AGuardianSimulator::AGuardianSimulator()
{
}

void AGuardianSimulator::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	if (bWantsToMove && !MoveDirection.IsZero())
	{
		GetPawn()->AddMovementInput(FVector(MoveDirection.X, MoveDirection.Y, 0.0f));
	}
	if (!TargetTile.IsValid() &&
		GameState.IsValid() &&
		GameState->GetColony() &&
		GameState->GetColony()->ClaimedTiles.Num() > 0)
	{
		TargetTile = GameState->GetColony()->ClaimedTiles[0];
	}
}

void AGuardianSimulator::BeginPlay()
{
	Super::BeginPlay();
	GameState = Cast<AGS_Ingame>(GetWorld()->GetGameState());
}

void AGuardianSimulator::SetMoveDirection(const FVector NewDirection)
{
	MoveDirection = NewDirection;
}

FVector AGuardianSimulator::GetMoveDirection()
{
	return MoveDirection;
}

bool AGuardianSimulator::GetIsMoving()
{
	return bWantsToMove;
}

void AGuardianSimulator::SetIsMoving(bool NewIsMoving)
{
	bWantsToMove = NewIsMoving;
}

ATile* AGuardianSimulator::GetTargetTile()
{
	return TargetTile.Get();
}

void AGuardianSimulator::ResetToRandomTile()
{
	if (!TargetTile.IsValid() &&
		GameState.IsValid() &&
		GameState->GetTileMap())
	{
		const ATile* RandomTile = GameState->GetTileMap()->GetRandomTile();
		GetPawn()->TeleportTo(RandomTile->GetActorTransform().GetLocation(),
		           RandomTile->GetActorTransform().GetRotation().Rotator());
	}
}
