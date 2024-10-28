#include "Colony.h"

#include "ColonyBrainSettings.h"
#include "GOTA/CoreSystems/Faction/Building/Building.h"
#include "GOTA/CoreSystems/Faction/Building/BuildingSettings.h"
#include "GOTA/CoreSystems/GameplayFramework/GameSettings.h"
#include "GOTA/CoreSystems/GameplayFramework/GS_Ingame.h"
#include "GOTA/CoreSystems/Tile/Tile.h"


void AColony::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if (ColonyBrainSettings)
	{
		FigureOutBuilding();
	}
	else
	{
		InitColonyBrainSettings();
	}
}

void AColony::InitColonyBrainSettings()
{
	ColonyBrainSettings = GetWorld()->GetGameState<AGS_Ingame>()->GetGameSettings()->GetColonyBrainSettings();
}

void AColony::FigureOutBuilding()
{
	if (!ShouldBuild()) return;
	ATile* Tile = FindBuildableTile();
	if (!Tile) return;
	UBuildingSettings* NewBuilding = SelectNewBuilding();
	Tile->TryBuild(NewBuilding, this);
}

bool AColony::ShouldBuild() const
{
	for (const ATile* Tile : ClaimedTiles)
	{
		if (Tile->GetBuilding()->GetIsUnderConstruction())
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
	TArray<UBuildingSettings*> PossibleBuildings = ColonyBrainSettings->GetPossibleBuildings();
	return PossibleBuildings[FMath::RandRange(0, PossibleBuildings.Num() - 1)];
	// TODO: Proper logic for figuring out building
	//CalculateImportances();
	//CalculateScores();
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
	const float MaxIncome = Data->Housing * Data->IncomeTime;
	if (Data->IncomeType == EProductionType::Food)
	{
		GainsScore = ImportanceRatings.Food * MaxIncome;
	}
	if (Data->IncomeType == EProductionType::Wood)
	{
		GainsScore = ImportanceRatings.Wood * MaxIncome;
	}
	if (Data->IncomeType == EProductionType::Stone)
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
