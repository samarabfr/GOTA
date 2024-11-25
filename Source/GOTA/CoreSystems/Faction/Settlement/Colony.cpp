#include "Colony.h"

#include "GOTA/CoreSystems/Entity/Army.h"
#include "GOTA/CoreSystems/Faction/Building/Building.h"
#include "GOTA/CoreSystems/Faction/Building/BuildingSettings.h"
#include "GOTA/CoreSystems/Tile/Tile.h"

AColony::AColony()
{
	Affiliation = EAffiliation::Enemy;
	SendArmiesIntervalTimeLeft = Settings->SendArmiesIntervalTime;
}

void AColony::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
}

void AColony::S_Tick(const float DeltaSeconds)
{
	C_Tick(DeltaSeconds);
	FigureOutBuilding();
	if(SendArmiesIntervalTimeLeft <= 0.0f)
	{
		SendArmies();
		SendArmiesIntervalTimeLeft  = Settings->SendArmiesIntervalTime;
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
	if(HasAuthority())
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
	return Settings->C_PossibleBuildings[FMath::RandRange(0, Settings->C_PossibleBuildings.Num() - 1)];
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
		if(Tile && Tile->GetBuilding() && Tile->GetBuilding()->GetArmy())
		{
			Result.Add(Tile->GetBuilding()->GetArmy());
		}
	}
	return Result;
}
