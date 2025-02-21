#include "Colony.h"

#include "GOTA/CoreSystems/Entity/Army.h"
#include "GOTA/CoreSystems/Faction/Building/Building.h"
#include "GOTA/CoreSystems/Faction/Building/BuildingSettings.h"
#include "GOTA/CoreSystems/Tile/Tile.h"

void AColony::S_Tick(const float DeltaSeconds)
{
	C_Tick(DeltaSeconds);
	FigureOutBuilding();
	if (SendArmiesIntervalTimeLeft <= 0.0f)
	{
		SendArmies();
		SendArmiesIntervalTimeLeft = SendArmiesIntervalTime;
	}
	else
	{
		SendArmiesIntervalTimeLeft -= DeltaSeconds;
	}
}

void AColony::C_Tick(const float DeltaSeconds)
{
}

void AColony::BeginDestroy()
{
	Super::BeginDestroy();
}

void AColony::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	if (HasAuthority())
		S_Tick(DeltaSeconds);
	else
		C_Tick(DeltaSeconds);
}

void AColony::FigureOutBuilding()
{
	if (!ShouldBuild()) return;
	ATile* Tile = FindBuildableTile();
	if (!Tile) return;
	UBuildingSettings* NewBuilding = SelectNewBuilding();
	if (!NewBuilding) return;
	Tile->S_TryBuild(NewBuilding, this);
}

bool AColony::ShouldBuild() const
{
	for (const ATile* Tile : ClaimedTiles)
	{
		if (Tile && Tile->GetBuilding()->GetIsUnderConstruction())
			return false;
	}
	return true;
}

ATile* AColony::FindBuildableTile() const
{
	if (BorderingUnclaimedTiles.Num() <= 0) return nullptr;
	return BorderingUnclaimedTiles[FMath::RandRange(0, BorderingUnclaimedTiles.Num() - 1)];
}

UBuildingSettings* AColony::SelectNewBuilding() const
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
		const auto [FoodIncome, WoodIncome, StoneIncome] = GetEffectivePredictedProduction();
		const int32 FoodThreshhold = 300;
		if (!ViableBuildings[i] ||
			(ViableBuildings[i]->Cost.Food > GetResources().Food && FoodIncome <= 0) ||
			(ViableBuildings[i]->Cost.Stone > GetResources().Stone && StoneIncome <= 0) ||
			(ViableBuildings[i]->Cost.Wood > GetResources().Wood && WoodIncome <= 0) ||
			((FoodIncome <= 0 || GetResources().Food < FoodThreshhold) &&
				ViableBuildings[i]->ProductionType != EProductionType::Food))
		{
			ViableBuildings.RemoveAt(i);
		}
	}
	if (ViableBuildings.Num() <= 0) return nullptr;
	return ViableBuildings[FMath::RandRange(0, ViableBuildings.Num() - 1)];
}

float AColony::CalculateScore(const UBuildingSettings* Data, FNewBuildingImportanceRatings ImportanceRatings)
{
	// costs
	float CostScore = 0;
	CostScore += FMath::Pow(EULERS_NUMBER, -0.1 * Data->Cost.Food);
	CostScore += FMath::Pow(EULERS_NUMBER, -0.1 * Data->Cost.Wood);
	CostScore += FMath::Pow(EULERS_NUMBER, -0.1 * Data->Cost.Stone);
	// Calculate GainScore
	float GainsScore = 0;
	// income
	const float MaxIncome = Data->Housing * Data->DirectProductionTime;
	if (Data->ProductionType == EProductionType::Food)
	{
		GainsScore = ImportanceRatings.Food * MaxIncome;
	}
	if (Data->ProductionType == EProductionType::Wood)
	{
		GainsScore = ImportanceRatings.Wood * MaxIncome;
	}
	if (Data->ProductionType == EProductionType::Stone)
	{
		GainsScore = ImportanceRatings.Food * MaxIncome;
	}

	// result
	return CostScore * GainsScore;
}

FNewBuildingImportanceRatings AColony::CalculateImportanceRatings() const
{
	FNewBuildingImportanceRatings ImportanceRatings;
	/*
	// The less income, the more important
	// food
	const float FoodIncome = BuildingSummary->ProductionMap[EProductionType::Food];
	ImportanceRatings.Food = Settings->FoodImportance
		* FMath::Pow(EULERS_NUMBER, -Settings->FoodImportanceDescent * FoodIncome);
	// wood
	const float WoodIncome = BuildingSummary->ProductionMap[EProductionType::Wood];
	ImportanceRatings.Wood = Settings->WoodImportance
		* FMath::Pow(EULERS_NUMBER, -Settings->WoodImportanceDescent * WoodIncome);
	// stone
	const float StoneIncome = BuildingSummary->ProductionMap[EProductionType::Stone];
	ImportanceRatings.Stone = Settings->StoneImportance
		* FMath::Pow(EULERS_NUMBER, -Settings->StoneImportanceDescent * StoneIncome);

	*/
	return ImportanceRatings;
}

void AColony::CalculateScores(FNewBuildingImportanceRatings ImportanceRatings)
{
	/*
	// calculate scores
	Scores.Empty();
	for (UBuildingProject* BuildingProject : BuildingProjectPool)
	{
		Scores.Add(FBuildingProjectScore(BuildingProject,
										 BuildingProject->CalculateScore(),
										 BuildingProject->Data,
										 BuildingProject->CalculateProjectTime()));
	}
	// sort by highest score
	Scores.Sort([](const FBuildingProjectScore& A, const FBuildingProjectScore& B)
	{
		return A.Score > B.Score;
	});
	// Set from the highest score
	UBuildingProject* Highest = nullptr;
	float LowestScore = -MAX_FLT;
	for (auto Score : Scores)
	{
		if (Score.Score > LowestScore)
		{
			LowestScore = Score.Score;
			Highest = Score.BuildingProject;
		}
	}
	SetCurrentBuildingProject(Highest);
	*/
}

void AColony::SendArmies()
{
	for (AArmy* Army : GetAllColonyArmies())
	{
		Army->SetMode(EArmyMode::AttackMode);
	}
}

TArray<AArmy*> AColony::GetAllColonyArmies()
{
	TArray<AArmy*> Result;
	for (ATile* Tile : ClaimedTiles)
	{
		if (Tile && Tile->GetBuilding() && Tile->GetBuilding()->GetArmy())
		{
			Result.Add(Tile->GetBuilding()->GetArmy());
		}
	}
	return Result;
}
