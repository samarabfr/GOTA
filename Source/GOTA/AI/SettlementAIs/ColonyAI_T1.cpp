#include "ColonyAI_T1.h"

#include "GOTA/Settlement/Settlement.h"
#include "GOTA/Tile/Building/Building.h"
#include "GOTA/Tile/Building/BuildingSettings.h"
#include "GOTA/Tile/Tile.h"

AColonyAI_T1::AColonyAI_T1()
{
}

void AColonyAI_T1::S_Tick(const float DeltaSeconds)
{
	FigureOutBuilding();
	if (SendArmiesIntervalTimeLeft <= 0.0f)
	{
		SendArmiesIntervalTimeLeft = SendArmiesIntervalTime;
		if (GetPossessedSettlement())
		{
			GetPossessedSettlement()->SetAllArmiesOnAttack();
		}
	}
	else
	{
		SendArmiesIntervalTimeLeft -= DeltaSeconds;
	}
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
	if (!GetPossessedSettlement())
			return false;
	for (const ATile* Tile : GetPossessedSettlement()->ClaimedTiles)
	{
		if (Tile && Tile->GetBuilding()->GetIsUnderConstruction())
			return false;
	}
	return true;
}

ATile* AColonyAI_T1::FindBuildableTile() const
{
	if (!GetPossessedSettlement())
		return nullptr;
	if (GetPossessedSettlement()->BorderingUnclaimedTiles.Num() <= 0)
		return nullptr;
	const int32 Index = FMath::RandRange(0, GetPossessedSettlement()->BorderingUnclaimedTiles.Num() - 1);
	return GetPossessedSettlement()->BorderingUnclaimedTiles[Index];
}

UBuildingSettings* AColonyAI_T1::SelectNewBuilding() const
{
	TArray<UBuildingSettings*> ViableBuildings = PossibleBuildings;
	// prevent soft-locking
	// 1. short-term: dont place buildings you cant build
	//		no buildings that require resources that you dont have on you and you have no income for
	//		build only food buildings when negative income or under certain threshhold
	// 2. long-term: have enough income of everything as a buffer and to grow fast
	//		1. prio: food; all buildings need food income
	//		2. prio: wood; all buildings need wood one time
	//		3. prio: stone; all military buildings need stone
	for (int i = ViableBuildings.Num() - 1; i >= 0; --i)
	{
		const auto [FoodIncome, WoodIncome, StoneIncome] = GetPossessedSettlement()->GetEffectiveProduction();
		const int32 FoodThreshhold = 300;
		if (!ViableBuildings[i] ||
			(ViableBuildings[i]->Cost.Food > 0 &&
				ViableBuildings[i]->Cost.Food > GetPossessedSettlement()->GetResources().Food && FoodIncome <= 0) ||
			(ViableBuildings[i]->Cost.Stone > 0 &&
				ViableBuildings[i]->Cost.Stone > GetPossessedSettlement()->GetResources().Stone && StoneIncome <= 0) ||
			(ViableBuildings[i]->Cost.Wood > 0 &&
				ViableBuildings[i]->Cost.Wood > GetPossessedSettlement()->GetResources().Wood && WoodIncome <= 0) ||
			((FoodIncome <= 0 || GetPossessedSettlement()->GetResources().Food < FoodThreshhold) &&
				ViableBuildings[i]->ProductionType != EProductionType::Food))
		{
			ViableBuildings.RemoveAt(i);
		}
	}
	if (ViableBuildings.Num() <= 0) return nullptr;
	return ViableBuildings[FMath::RandRange(0, ViableBuildings.Num() - 1)];
}

void AColonyAI_T1::LogBuildings()
{
	for (auto Counter : BuildingsCounter)
	{
		UE_LOG(LogTemp, Warning, TEXT("%s: %d"), *Counter.Key.ToString(), Counter.Value)
	}
}

void AColonyAI_T1::Log()
{
	UE_LOG(LogTemp, Warning, TEXT("-------------------------- Colony AI - T1--------------------------"))
	LogBuildings();
}
