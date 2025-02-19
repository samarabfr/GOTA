// Fill out your copyright notice in the Description page of Project Settings.


#include "GuardianAI_BuildingSelector.h"

#include "LearningAgentsManager.h"
#include "GOTA/CoreSystems/GameplayFramework/GS_Ingame.h"
#include "GOTA/CoreSystems/Tile/TileMap.h"
#include "GOTA/ReinforcementLearning/BuildingSelector/RL_BuildingSelectorManager.h"

AGuardianAI_BuildingSelector::AGuardianAI_BuildingSelector()
{
}


void AGuardianAI_BuildingSelector::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
}

void AGuardianAI_BuildingSelector::BeginPlay()
{
	Super::BeginPlay();
	if (HasAuthority())
	{
		GameState = Cast<AGS_Ingame>(GetWorld()->GetGameState());
		ARL_BuildingSelectorManager* Manager = GameState->S_GetRLManager<ARL_BuildingSelectorManager>(ManagerClass);
		if (!Manager)
		{
			Manager = GetWorld()->SpawnActor<ARL_BuildingSelectorManager>(ManagerClass,
				FVector::Zero(), FRotator::ZeroRotator);
			Manager->S_Init(NN_Encoder, NN_Policy, NN_Decoder, NN_Critic);
			AddTickPrerequisiteActor(Manager); // make the manager tick before this
			GameState->S_AddManager(ManagerClass, Manager);
		}
		Manager->S_RegisterAgent(this);
	}
}
