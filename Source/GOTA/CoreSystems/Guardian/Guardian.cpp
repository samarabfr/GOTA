// Fill out your copyright notice in the Description page of Project Settings.


#include "Guardian.h"
#include "GOTA/CoreSystems/GameplayFramework/LoadingManager.h"

void AGuardian::BeginPlay()
{
	Super::BeginPlay();
	// IncreaseReplicationCount for LoadingProcess
	AGS_Ingame* GameState = GetWorld()->GetGameState<AGS_Ingame>();
	GameState->LoadingManager->IncrementReplicationCount();

	MouseUtils = GetWorld()->SpawnActor<AMouseUtils>(MouseUtilsClass);
	SetupGAM();
}

AMouseUtils* AGuardian::GetMouseUtils()
{
	return MouseUtils;
}
