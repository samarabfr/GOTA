// Fill out your copyright notice in the Description page of Project Settings.


#include "SimulatedGuardian.h"

void ASimulatedGuardian::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	if (bWantsToMove && !MoveDirection.IsZero())
	{
		AddMovementInput(FVector(MoveDirection.X, MoveDirection.Y, 0.0f));
	}
}
