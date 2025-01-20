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
	if (bWantsToMove)
	{
		GetPawn()->AddMovementInput(GetPawn()->GetActorForwardVector());
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
	if (GameState.IsValid() && GameState->GetTileMap())
	{
		const ATile* RandomTile = GameState->GetTileMap()->GetRandomTile();
		GetPawn()->TeleportTo(RandomTile->GetActorTransform().GetLocation(),
		           RandomTile->GetActorTransform().GetRotation().Rotator());
	}
}

void AGuardianSimulator::SteerPawn(float SteeringAngle)
{
	if (SteeringAngle == 0.f) return;
	GetPawn()->AddActorLocalRotation(FRotator(0.f, SteeringAngle, 0.f));
}
