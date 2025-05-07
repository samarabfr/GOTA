// Fill out your copyright notice in the Description page of Project Settings.


#include "GuardianAI_RandomNoSoftLock.h"

#include "GOTA/GameplayFramework/GS_Ingame.h"
#include "GOTA/Guardian/Guardian.h"
#include "GOTA/Settlement/Settlement.h"
#include "GOTA/Tile/Tile.h"
#include "GOTA/Tile/Building/BuildingSettings.h"
#include "GOTA/Tile/Building/Population.h"


void AGuardianAI_RandomNoSoftLock::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	if (HasAuthority())
		S_Tick(DeltaSeconds);
	else
		C_Tick(DeltaSeconds);
}

AGuardianAI_RandomNoSoftLock::AGuardianAI_RandomNoSoftLock()
{
}

void AGuardianAI_RandomNoSoftLock::S_Tick(const float DeltaSeconds)
{
	FigureOutBuilding();
}

void AGuardianAI_RandomNoSoftLock::C_Tick(const float DeltaSeconds)
{
}

void AGuardianAI_RandomNoSoftLock::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
	if (HasAuthority())
	{
		GameState = Cast<AGS_Ingame>(GetWorld()->GetGameState());
		PossessedGuardian = Cast<AGuardian>(InPawn);
		Settlement = GameState->GetTribe();
		// Buildingscounter
		if (!PossessedGuardian)
			return;
		BuildingsCounter.Empty();
		for (UBuildingSettings* Building : PossessedGuardian->GetPossibleBuildings())
		{
			BuildingsCounter.Add(Building->Name, 0);
		}
	}
}

void AGuardianAI_RandomNoSoftLock::FigureOutBuilding()
{
	if (!IsValid(Settlement)) // valid check because the settlement might be pending kill
		return;
	if (!ShouldBuild()) return;
	ATile* Tile = FindBuildableTile();
	if (!Tile) return;
	UBuildingSettings* NewBuilding = SelectNewBuilding();
	if (!NewBuilding) return;
	if (Tile && Tile->CanBuild(NewBuilding, Settlement))
	{
		if (Tile->S_TryBuild(NewBuilding, Settlement))
		{
			BuildingsCounter[NewBuilding->Name]++;
		}
	}
}

bool AGuardianAI_RandomNoSoftLock::ShouldBuild() const
{
	if (!IsValid(Settlement)) // valid check because the settlement might be pending kill
		return false;
	return Settlement->GetCountOfConstructionSites() == 0 && Settlement->CanAddConstructionSite();
}

ATile* AGuardianAI_RandomNoSoftLock::FindBuildableTile() const
{
	if (!IsValid(Settlement)) // valid check because the settlement might be pending kill
		return nullptr;
	if (Settlement->BorderingUnclaimedTiles.Num() <= 0)
		return nullptr;
	TArray<ATile*> BestTiles = FindTilesWithMostNeighborPop();
	return BestTiles[FMath::RandRange(0, BestTiles.Num() - 1)];
}

TArray<ATile*> AGuardianAI_RandomNoSoftLock::FindTilesWithMostNeighborPop() const
{
	TMap<ATile*, int32> NeighborPops;
	// Find max Neighbor pop
	int32 MaxNeighborPop = 0;
	for (ATile* BorderingUnclaimedTile : Settlement->BorderingUnclaimedTiles)
	{
		int32 NeighborPop = 0;
		for (ATile* Neighbor : BorderingUnclaimedTile->Neighbors)
		{
			if (Neighbor && Neighbor->GetClaimant() &&
				Neighbor->GetClaimant()->GetAffiliation() == Settlement->GetAffiliation() &&
				Neighbor->GetBuilding()->GetPopulation()->GetSize() > MaxNeighborPop)
			{
				NeighborPop += Neighbor->GetBuilding()->GetPopulation()->GetSize();
			}
		}
		NeighborPops.Add(BorderingUnclaimedTile, NeighborPop);
		if (NeighborPop > MaxNeighborPop)
		{
			MaxNeighborPop = NeighborPop;
		}
	}
	// Get all tiles with max neighbor pop
	TArray<ATile*> TilesWithMostNeighborPop;
	for (auto NeighborPopTile : NeighborPops)
	{
		if (NeighborPopTile.Value == MaxNeighborPop)
		{
			TilesWithMostNeighborPop.Add(NeighborPopTile.Key);
		}
	}
	return TilesWithMostNeighborPop;
}

UBuildingSettings* AGuardianAI_RandomNoSoftLock::SelectNewBuilding() const
{
	if (!GameState || !PossessedGuardian)
		return nullptr;
	TArray<UBuildingSettings*> ViableBuildings = PossessedGuardian->GetPossibleBuildings();
	// Remove buildings that cant be build
	for (int i = ViableBuildings.Num() - 1; i >= 0; --i)
	{
		const FConstructionResources Income = Settlement->GetEffectiveProduction();
		const FConstructionResources MaxIncome = Settlement->GetMaximumEffectiveProduction();
		if (!ViableBuildings[i])
		{
			ViableBuildings.RemoveAt(i);
			continue;
		}
		// doesnt have resources for the building
		if (ViableBuildings[i]->Cost.Food > 0 &&
			ViableBuildings[i]->Cost.Food > Settlement->GetResources().Food &&
			Income.Food <= 0)
		{
			ViableBuildings.RemoveAt(i);
			continue;
		}
		if (ViableBuildings[i]->Cost.Stone > 0 &&
			ViableBuildings[i]->Cost.Stone > Settlement->GetResources().Stone &&
			MaxIncome.Stone <= 0)
		{
			ViableBuildings.RemoveAt(i);
			continue;
		}
		if (ViableBuildings[i]->Cost.Wood > 0 &&
			ViableBuildings[i]->Cost.Wood > Settlement->GetResources().Wood &&
			MaxIncome.Wood <= 0)
		{
			ViableBuildings.RemoveAt(i);
			continue;
		}
		// if food income is too low build food buildings
		if ((Income.Food <= 0 || MaxIncome.Food < FoodBuildingFoodIncomeThreshold) &&
			ViableBuildings[i]->ProductionType != EProductionType::Food)
		{
			ViableBuildings.RemoveAt(i);
			continue;
		}
		// dont build army buildings, if income is not high enough or there are too many unprotected buildings
		const int32 CountUnprotectedBuildings = Settlement->GetCountOfUnprotectedBuildings();
		if (ViableBuildings[i]->bArmyEnabled &&
			(!(MaxIncome > BarracksIncomeThreshold) ||
				CountUnprotectedBuildings >= BarracksUnprotectedBuildingsThreshold))
		{
			ViableBuildings.RemoveAt(i);
			continue;
		}
		// dont build defense buildings, if there are no unprotected buildings
		if (ViableBuildings[i]->bDefenseEnabled &&
			CountUnprotectedBuildings <= 0)
		{
			ViableBuildings.RemoveAt(i);
			continue;
		}
	}
	// only build barracks, if they are valid
	bool bArmyBuildingsValid = false;
	for (UBuildingSettings* ViableBuilding : ViableBuildings)
	{
		if (ViableBuilding->bArmyEnabled)
		{
			bArmyBuildingsValid = true;
			break;
		}
	}
	if (bArmyBuildingsValid)
	{
		for (int i = ViableBuildings.Num() - 1; i >= 0; --i)
		{
			if (!ViableBuildings[i]->bArmyEnabled)
			{
				ViableBuildings.RemoveAt(i);
			}
		}
	}
	// return random viable building
	if (ViableBuildings.Num() <= 0) return nullptr;
	return ViableBuildings[FMath::RandRange(0, ViableBuildings.Num() - 1)];
}

TSharedPtr<FJsonObject> AGuardianAI_RandomNoSoftLock::Log()
{
	TSharedPtr<FJsonObject> NewLog = MakeShareable(new FJsonObject());
	// TODO: Add GotaID
	for (auto Counter : BuildingsCounter)
	{
		NewLog->SetNumberField(Counter.Key.ToString(), Counter.Value);
	}
	return NewLog;
}
