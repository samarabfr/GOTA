#include "Colony.h"

#include "Algo/RandomShuffle.h"
#include "Net/UnrealNetwork.h"

AColony::AColony()
{
	Affiliation = EAffiliation::Enemy;
	
	CurrentBuildingProject = CreateDefaultSubobject<UBuildingProject>(TEXT("Current Building Project"));
}

void AColony::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	
	FigureOutBuilding();
}

void AColony::BeginPlay()
{
	Super::BeginPlay();

	if (HasAuthority())
	{
		AddReplicatedSubObject(CurrentBuildingProject);
	}
}

void AColony::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	FDoRepLifetimeParams Params;
	Params.bIsPushBased = true;

	Params.Condition = COND_None;
	Params.RepNotifyCondition = REPNOTIFY_OnChanged;
	DOREPLIFETIME_WITH_PARAMS(AColony, CurrentBuildingProject, Params);
}


void AColony::FigureOutBuilding()
{
	if (CurrentBuildingProject && CurrentBuildingProject->IsPossible())
	{
		if (CurrentBuildingProject->CanAfford())
		{
			CurrentBuildingProject->TryBuilding();
			SetCurrentBuildingProject(nullptr);
			SelectNewBuildingProject();
		}
	}
	else
	{
		SetCurrentBuildingProject(nullptr);
		SelectNewBuildingProject();
	}
}

void AColony::SelectNewBuildingProject()
{
	CalculateImportances();
	FillBuildingPool();
	if (BuildingProjectPool.IsEmpty()) return;
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
}

void AColony::FillBuildingPool()
{
	BuildingProjectPool.Empty();
	// If there is a free building slot, add all possible Buildings to the pool
	Algo::RandomShuffle(ClaimedTiles);
	for (ATile* ClaimedTile : ClaimedTiles)
	{
		if (ClaimedTile->Building) continue;
		for (UBuildingDataAsset* PossibleBuilding : PossibleBuildings)
		{
			UBuildingProject* BuildingProject = NewObject<UBuildingProject>();
			BuildingProject->Init(this, PossibleBuilding, ClaimedTile);
			BuildingProjectPool.Add(BuildingProject);
		}
		break;
	}
}

void AColony::CalculateImportances()
{
	// The less income, the more important
	// food
	float FoodIncome = BuildingSummary->ProductionMap[EProductionType::Food];
	ImportanceRatings.Food = SettlementSettings->FoodImportance
		* FMath::Pow(EULERS_NUMBER, -SettlementSettings->FoodImportanceDescent * FoodIncome);
	// wood
	float WoodIncome = BuildingSummary->ProductionMap[EProductionType::Wood];
	ImportanceRatings.Wood = SettlementSettings->WoodImportance
		* FMath::Pow(EULERS_NUMBER, -SettlementSettings->WoodImportanceDescent * WoodIncome);
	// stone
	float StoneIncome = BuildingSummary->ProductionMap[EProductionType::Stone];
	ImportanceRatings.Stone = SettlementSettings->StoneImportance
		* FMath::Pow(EULERS_NUMBER, -SettlementSettings->StoneImportanceDescent * StoneIncome);
}


void AColony::SetCurrentBuildingProject(UBuildingProject* NewCurrentBuildingProject)
{
	if (CurrentBuildingProject)
	{
		RemoveReplicatedSubObject(CurrentBuildingProject);
	}
	CurrentBuildingProject = NewCurrentBuildingProject;
	if (CurrentBuildingProject)
	{
		AddReplicatedSubObject(CurrentBuildingProject);
	}
}
