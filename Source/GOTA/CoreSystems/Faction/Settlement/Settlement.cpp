// Fill out your copyright notice in the Description page of Project Settings.

#include "Settlement.h"

#include "GOTA/CoreSystems/Entity/Army.h"
#include "GOTA/CoreSystems/Faction/Building/Building.h"
#include "GOTA/CoreSystems/GameplayFramework/GS_Ingame.h"
#include "GOTA/CoreSystems/GameplayFramework/LoadingManager.h"
#include "Net/UnrealNetwork.h"

void ASettlement::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	FDoRepLifetimeParams Params;
	Params.bIsPushBased = true;

	Params.Condition = COND_InitialOnly;
	Params.RepNotifyCondition = REPNOTIFY_Always;
	DOREPLIFETIME_WITH_PARAMS(ASettlement, Affiliation, Params);

	Params.Condition = COND_None;
	Params.RepNotifyCondition = REPNOTIFY_OnChanged;
	DOREPLIFETIME_WITH_PARAMS(ASettlement, Resources, Params);
}

ASettlement::ASettlement()
{
	bReplicates = true;
	bAlwaysRelevant = true;
	bReplicateUsingRegisteredSubObjectList = true;
	NetUpdateFrequency = 1.0f;

	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = false;
	PrimaryActorTick.TickInterval = 0.5;

	RootComponent = CreateDefaultSubobject<USceneComponent>("ROOT");

	PopulationSummary = CreateDefaultSubobject<USettlementPopulation>(TEXT("Population"));

	// Load Settlement Settings DataAsset
	ConstructorHelpers::FObjectFinder<USettlementSettings> DataAsset(
		TEXT("/Game/CoreSystems/Faction/DA_SettlementSettings"));
	Settings = DataAsset.Object;
}

void ASettlement::BeginPlay()
{
	Super::BeginPlay();
	GetWorld()->GetGameState<AGS_Ingame>()->LoadingManager->IncrementReplicationCount();
}

void ASettlement::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	
	UpdateLastMinuteResources();
}

void ASettlement::EnableTick()
{
	SetActorTickEnabled(true);
}

void ASettlement::StartingSetup(ATile* SpawnTile)
{
	const FGameResources& StartingResources = Affiliation == EAffiliation::Enemy
		                                          ? Settings->C_StartingResources
		                                          : Settings->N_StartingResources;
	const TArray<UBuildingDataAsset*>& StartingBuildings = Affiliation == EAffiliation::Enemy
		                                                       ? Settings->C_StartingBuildings
		                                                       : Settings->N_StartingBuildings;
	GameplayTags = Affiliation == EAffiliation::Enemy
		               ? Settings->C_GameplayTags
		               : Settings->N_GameplayTags;
	Resources += StartingResources;
	SpawnTile->TryBuild(StartingBuildings[0], this);
	for (int32 i = 1; i < StartingBuildings.Num(); ++i)
	{
		if (BorderingUnclaimedTiles.Num() <= 0) break;
		ATile* Tile = BorderingUnclaimedTiles[FMath::RandRange(0, BorderingUnclaimedTiles.Num() - 1)];
		Tile->TryBuild(StartingBuildings[i], this);
		Tile->Building->FinishConstruction();
	}
	for (ATile* Tile : ClaimedTiles)
	{
		Tile->Building->Population->ChangeSize(100);
	}
}

// -------------------Claims-------------------------

void ASettlement::RefreshBorderingUnclaimedTiles()
{
	BorderingUnclaimedTiles.Empty();
	for (ATile* ClaimedTile : ClaimedTiles)
	{
		for (ATile* Neighbor : ClaimedTile->Neighbors)
		{
			if (Neighbor && !Neighbor->GetClaimant() && !BorderingUnclaimedTiles.Contains(Neighbor))
			{
				BorderingUnclaimedTiles.Add(Neighbor);
			}
		}
	}
}

bool ASettlement::IsBorderingUnclaimedTile(const ATile* Tile) const
{
	return BorderingUnclaimedTiles.Contains(Tile);
}

// -------------------Building-------------------------

void ASettlement::OnBuildingAdded(UBuilding* Building, ATile* Tile)
{
	PopulationSummary->RegisterPop(Building->Population);
	ClaimedTiles.Add(Tile);
	RefreshBorderingUnclaimedTiles();
}

void ASettlement::OnBuildingRemoved(UBuilding* Building, ATile* Tile)
{
	PopulationSummary->UnregisterPop(Building->Population);
	ClaimedTiles.Remove(Tile);
	RefreshBorderingUnclaimedTiles();
}

// -------------------Resources-------------------------

void ASettlement::UpdateLastMinuteResources()
{
	const float CurrentCutOff = GetWorld()->GetTimeSeconds() - 20.0f;

	// Remove old entries from Income queue
	while (const FIncomeEvent* Tail = IncomeEvents.Peek())
	{
		if (Tail->Timestamp < CurrentCutOff)
		{
			LastMinuteIncome -= Tail->Amount;
			IncomeEvents.Pop();
		}
		else
		{
			break;
		}
	}

	// Remove old entries from Consumption queue
	while (const FIncomeEvent* Tail = ConsumptionEvents.Peek())
	{
		if (Tail->Timestamp < CurrentCutOff)
		{
			LastMinuteConsumption -= Tail->Amount;
			ConsumptionEvents.Pop();
		}
		else
		{
			break;
		}
	}
}

void ASettlement::AddResources(FGameResources Amount, bool CountTowardsIncomeLastMinute)
{
	Resources += Amount;
	if (CountTowardsIncomeLastMinute)
	{
		LastMinuteIncome += Amount;
		IncomeEvents.Enqueue(FIncomeEvent(Amount, GetWorld()->GetTimeSeconds()));
	}
}

void ASettlement::RemoveResources(FGameResources Amount, bool CountTowardsLastMinuteConsumption)
{
	Resources -= Amount;
	if (CountTowardsLastMinuteConsumption)
	{
		LastMinuteConsumption += Amount;
		ConsumptionEvents.Enqueue(FIncomeEvent(Amount, GetWorld()->GetTimeSeconds()));
	}
}
