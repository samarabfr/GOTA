// Fill out your copyright notice in the Description page of Project Settings.


#include "Guardian.h"
#include "Net/UnrealNetwork.h"
#include "GOTA/CoreSystems/GameplayFramework/LoadingManager.h"

void AGuardian::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	
}

void AGuardian::BeginPlay()
{
	Super::BeginPlay();
	// IncreaseReplicationCount for LoadingProcess
	AGS_Ingame* GameState = GetWorld()->GetGameState<AGS_Ingame>();
	GameState->LoadingManager->IncrementReplicationCount();
	
	if (HasAuthority())
	{
		SetupGAM();
	}
}