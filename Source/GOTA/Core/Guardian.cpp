// Fill out your copyright notice in the Description page of Project Settings.


#include "Guardian.h"
#include "GOTA/Core/GOTAGameState.h"
#include "GOTA/Core/LoadingManager.h"

void AGuardian::BeginPlay()
{
	Super::BeginPlay();
	
	// Get the GameState
	AGOTAGameState* GameState = GetWorld()->GetGameState<AGOTAGameState>();
	GameState->LoadingManager->IncrementReplicationCount();
}