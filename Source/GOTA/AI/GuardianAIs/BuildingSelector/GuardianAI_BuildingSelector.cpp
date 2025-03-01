// Fill out your copyright notice in the Description page of Project Settings.


#include "GuardianAI_BuildingSelector.h"

#include "LearningAgentsManager.h"
#include "GOTA/CoreSystems/Faction/Settlement/Tribe.h"
#include "GOTA/CoreSystems/GameplayFramework/GS_Ingame.h"
#include "GOTA/CoreSystems/Guardian/Guardian.h"
#include "GOTA/CoreSystems/Tile/TileMap.h"
#include "GOTA/ReinforcementLearning/BuildingSelector/RL_BuildingSelectorManager.h"


void AGuardianAI_BuildingSelector::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
}

AGuardianAI_BuildingSelector::AGuardianAI_BuildingSelector()
{
}

void AGuardianAI_BuildingSelector::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
	if (HasAuthority())
	{
		GameState = Cast<AGS_Ingame>(GetWorld()->GetGameState());
		ARL_BuildingSelectorManager* Manager = GameState->S_GetRLManager<ARL_BuildingSelectorManager>(ManagerClass);
		PossessedGuardian = Cast<AGuardian>(InPawn);
		Settlement = GameState->GetTribe();
		MilestonesReached.SetNumZeroed(6);
		if (!Manager)
		{
			Manager = GetWorld()->SpawnActor<ARL_BuildingSelectorManager>(ManagerClass,
			                                                              FVector::Zero(),
			                                                              FRotator::ZeroRotator);
			Manager->S_Init(NN_Encoder, NN_Policy, NN_Decoder, NN_Critic,
			                PossessedGuardian->GetPossibleBuildings().Num());
			AddTickPrerequisiteActor(Manager); // make the manager tick before this
			GameState->S_AddManager(ManagerClass, Manager);
		}
		if (!Manager->IsRegistered(this))
		{
			Manager->S_RegisterAgent(this);
		}
		if (Manager->IsPaused())
		{
			Manager->Unpause();
		}
	}
}

void AGuardianAI_BuildingSelector::RandomlyPlaceBuilding(UBuildingSettings* Building)
{
	if (Settlement->BorderingUnclaimedTiles.Num() <= 0) return;
	const int32 RandomIndex = FMath::RandRange(0, Settlement->BorderingUnclaimedTiles.Num() - 1);
	ATile* Tile = Settlement->BorderingUnclaimedTiles[RandomIndex];
	if (Tile && Tile->CanBuild(Building, Settlement))
	{
		Tile->S_TryBuild(Building, Settlement);
	}
}

ASettlement* AGuardianAI_BuildingSelector::GetSettlement()
{
	return Settlement;
}

TArray<UBuildingSettings*> AGuardianAI_BuildingSelector::GetAvailableBuildings()
{
	if (!PossessedGuardian) return TArray<UBuildingSettings*>();
	return PossessedGuardian->GetPossibleBuildings();
}

void AGuardianAI_BuildingSelector::SelectBuilding(UBuildingSettings* Building)
{
	RandomlyPlaceBuilding(Building);
}

EAffiliation AGuardianAI_BuildingSelector::GetAffiliation()
{
	return EAffiliation::Ally;
}

TArray<int32> AGuardianAI_BuildingSelector::GetMilestonesReached()
{
	return MilestonesReached;
}

void AGuardianAI_BuildingSelector::IncrementMilestone(int32 MilestoneIndex)
{
	if (MilestoneIndex >= MilestonesReached.Num()) return;
	MilestonesReached[MilestoneIndex] += 1;
}
