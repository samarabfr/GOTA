#include "ColonyAI_T1.h"

#include "GOTA/Settlement/Settlement.h"
#include "GOTA/Tile/Building/Building.h"
#include "GOTA/Tile/Building/BuildingSettings.h"
#include "GOTA/Tile/Tile.h"
#include "GOTA/Tile/Building/Population.h"

AColonyAI_T1::AColonyAI_T1()
{
}

void AColonyAI_T1::S_Tick(const float DeltaSeconds)
{
	FigureOutBuilding();
}

void AColonyAI_T1::C_Tick(const float DeltaSeconds)
{
}

void AColonyAI_T1::Possess(ASettlement* Settlement)
{
	Super::Possess(Settlement);
	BuildingsCounter.Empty();
	for (UBuildingSettings* Building : PossibleBuildings)
	{
		BuildingsCounter.Add(Building->Name, 0);
	}
}

void AColonyAI_T1::BeginDestroy()
{
	Super::BeginDestroy();
}

void AColonyAI_T1::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	if (HasAuthority())
		S_Tick(DeltaSeconds);
	else
		C_Tick(DeltaSeconds);
}

void AColonyAI_T1::FigureOutBuilding()
{
	if (!GetPossessedSettlement())
		return;
	if (!ShouldBuild()) return;
	ATile* Tile = FindBuildableTile();
	if (!Tile) return;
	UBuildingSettings* NewBuilding = SelectNewBuilding();
	if (!NewBuilding) return;
	if (Tile && Tile->CanBuild(NewBuilding, GetPossessedSettlement()))
	{
		if (Tile->S_TryBuild(NewBuilding, GetPossessedSettlement()))
		{
			BuildingsCounter[NewBuilding->Name]++;
		}
	}
}

bool AColonyAI_T1::ShouldBuild() const
{
	if (!GetPossessedSettlement()) // valid check because the settlement might be pending kill
		return false;
	return GetPossessedSettlement()->GetCountOfConstructionSites() == 0 &&
		GetPossessedSettlement()->CanAddConstructionSite();
}

ATile* AColonyAI_T1::FindBuildableTile() const
{
	if (!GetPossessedSettlement())
		return nullptr;
	if (GetPossessedSettlement()->BorderingUnclaimedTiles.Num() <= 0)
		return nullptr;
	TArray<ATile*> BestTiles = FindTilesWithMostNeighborPop();
	return BestTiles[FMath::RandRange(0, BestTiles.Num() - 1)];
}

TArray<ATile*> AColonyAI_T1::FindTilesWithMostNeighborPop() const
{
	TMap<ATile*, int32> NeighborPops;
	// Find max Neighbor pop
	int32 MaxNeighborPop = 0;
	for (ATile* BorderingUnclaimedTile : GetPossessedSettlement()->BorderingUnclaimedTiles)
	{
		int32 NeighborPop = 0;
		for (ATile* Neighbor : BorderingUnclaimedTile->Neighbors)
		{
			if (Neighbor && Neighbor->GetClaimant() &&
				Neighbor->GetClaimant()->GetAffiliation() == GetPossessedSettlement()->GetAffiliation() &&
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

UBuildingSettings* AColonyAI_T1::SelectNewBuilding() const
{
	TArray<UBuildingSettings*> ViableBuildings = PossibleBuildings;
	// Remove buildings that cant be build
	for (int i = ViableBuildings.Num() - 1; i >= 0; --i)
	{
		const FConstructionResources Income = GetPossessedSettlement()->GetEffectiveProduction();
		const FConstructionResources MaxIncome = GetPossessedSettlement()->GetMaximumEffectiveProduction();
		if (!ViableBuildings[i])
		{
			ViableBuildings.RemoveAt(i);
			continue;
		}
		// doesnt have resources for the building
		if (ViableBuildings[i]->Cost.Food > 0 &&
			ViableBuildings[i]->Cost.Food > GetPossessedSettlement()->GetResources().Food &&
			MaxIncome.Food <= 0)
		{
			ViableBuildings.RemoveAt(i);
			continue;
		}
		if (ViableBuildings[i]->Cost.Stone > 0 &&
			ViableBuildings[i]->Cost.Stone > GetPossessedSettlement()->GetResources().Stone &&
			MaxIncome.Stone <= 0)
		{
			ViableBuildings.RemoveAt(i);
			continue;
		}
		if (ViableBuildings[i]->Cost.Wood > 0 &&
			ViableBuildings[i]->Cost.Wood > GetPossessedSettlement()->GetResources().Wood &&
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
		const int32 CountUnprotectedBuildings = GetPossessedSettlement()->GetCountOfUnprotectedBuildings();
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

TSharedPtr<FJsonObject> AColonyAI_T1::Log()
{
	TSharedPtr<FJsonObject> NewLog = MakeShareable(new FJsonObject());
	for (auto Counter : BuildingsCounter)
	{
		NewLog->SetNumberField(Counter.Key.ToString(), Counter.Value);
	}
	return NewLog;
}
