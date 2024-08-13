// Fill out your copyright notice in the Description page of Project Settings.


#include "BuildingProject.h"
#include "Settlement.h"
#include "GOTA/CoreSystems/Faction/Building/Building.h"
#include "Net/UnrealNetwork.h"

void UBuildingProject::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	UObject::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UBuildingProject, Builder);
	DOREPLIFETIME(UBuildingProject, Data);
	DOREPLIFETIME(UBuildingProject, Tier);
	DOREPLIFETIME(UBuildingProject, Tile);
	DOREPLIFETIME(UBuildingProject, Cost);
}

bool UBuildingProject::IsSupportedForNetworking() const
{
	return true;
}

bool UBuildingProject::IsPossible() const
{
	if (!Builder) return false;
	if (!Tile) return false;
	if (Tier < 1) return false;

	//Tile is not claimed by the Builder of this project
	if (Tile->GetClaimant() != Builder) return false;

	//want to build a new building, but tile already has a building
	if (Tier == 1 && Tile->Building) return false;

	//want to upgrade a building but there is no building
	if (Tier > 1 && !Tile->Building) return false;

	//want to upgrade a building, but the tier of the building is not the previous tier of this project
	if (Tier > 1 && Tile->Building->Tier != Tier - 1) return false;

	return true;
}

bool UBuildingProject::CanAfford() const
{
	if (Cost.Wood > Builder->Wood->Current) return false;
	if (Cost.Stone > Builder->Stone->Current) return false;
	return true;
}

bool UBuildingProject::TryBuilding()
{
	// Trying to build a new building
	if (Tier == 1)
	{
		if(Tile->TryBuild(Data))
		{
			int32 EC = 0;
			if (Cost.Wood > 0) Builder->Wood->Subtract(Cost.Wood, EC);
			if (Cost.Stone > 0) Builder->Stone->Subtract(Cost.Stone, EC);
			return true;
		}
	}
	// Trying to upgrade a Building
	if (Tile->TryUpgrade())
	{
		int32 EC = 0;
		if (Cost.Wood > 0) Builder->Wood->Subtract(Cost.Wood, EC);
		if (Cost.Stone > 0) Builder->Stone->Subtract(Cost.Stone, EC);
		return true;
	}
	return false;
}

void UBuildingProject::Init(ASettlement* Builder_, UBuildingDataAsset* Data_, int32 Tier_, ATile* Tile_)
{
	Builder = Builder_;
	Data = Data_;
	Tier = Tier_;
	Tile = Tile_;
	Cost = Data->GetTierData(Tier)->Cost;
}