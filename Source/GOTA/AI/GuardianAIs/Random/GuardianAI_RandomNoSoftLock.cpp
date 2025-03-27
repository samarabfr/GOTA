// Fill out your copyright notice in the Description page of Project Settings.


#include "GuardianAI_RandomNoSoftLock.h"

#include "GOTA/GameplayFramework/GS_Ingame.h"
#include "GOTA/Guardian/Guardian.h"
#include "GOTA/Settlement/Settlement.h"
#include "GOTA/Tile/Tile.h"
#include "GOTA/Tile/Building/BuildingSettings.h"


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
	for (const ATile* Tile : Settlement->ClaimedTiles)
	{
		if (Tile && Tile->GetBuilding()->GetIsUnderConstruction())
			return false;
	}
	return true;
}

ATile* AGuardianAI_RandomNoSoftLock::FindBuildableTile() const
{
	if (!IsValid(Settlement)) // valid check because the settlement might be pending kill
		return nullptr;
	if (Settlement->BorderingUnclaimedTiles.Num() <= 0)
		return nullptr;
	const int32 Index = FMath::RandRange(0, Settlement->BorderingUnclaimedTiles.Num() - 1);
	return Settlement->BorderingUnclaimedTiles[Index];
}

UBuildingSettings* AGuardianAI_RandomNoSoftLock::SelectNewBuilding() const
{
	if (!GameState || !PossessedGuardian)
		return nullptr;
	TArray<UBuildingSettings*> ViableBuildings = PossessedGuardian->GetPossibleBuildings();
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
		const auto [FoodIncome, WoodIncome, StoneIncome] = Settlement->GetEffectiveProduction();
		if (!ViableBuildings[i] ||
			(ViableBuildings[i]->Cost.Food > 0 &&
				ViableBuildings[i]->Cost.Food > Settlement->GetResources().Food && FoodIncome <= 0) ||
			(ViableBuildings[i]->Cost.Stone > 0 &&
				ViableBuildings[i]->Cost.Stone > Settlement->GetResources().Stone && StoneIncome <= 0) ||
			(ViableBuildings[i]->Cost.Wood > 0 &&
				ViableBuildings[i]->Cost.Wood > Settlement->GetResources().Wood && WoodIncome <= 0) ||
			((FoodIncome <= 0 || Settlement->GetResources().Food < FoodThreshold) &&
				ViableBuildings[i]->ProductionType != EProductionType::Food))
		{
			ViableBuildings.RemoveAt(i);
		}
	}
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
