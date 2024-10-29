// Fill out your copyright notice in the Description page of Project Settings.

#include "Settlement.h"

#include "GOTA/CoreSystems/Entity/Army.h"
#include "GOTA/CoreSystems/Faction/Building/Building.h"
#include "GOTA/CoreSystems/Faction/Building/PopulationSettings.h"
#include "GOTA/CoreSystems/GameplayFramework/GS_Ingame.h"
#include "GOTA/CoreSystems/GameplayFramework/LoadingManager.h"
#include "Net/UnrealNetwork.h"
#include "Net/Core/PushModel/PushModel.h"

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

// ------------------- LifeCycle -------------------

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

	Population = CreateDefaultSubobject<USettlementPopulation>(TEXT("Population"));

	// Load Settlement Settings DataAsset
	static ConstructorHelpers::FObjectFinder<USettlementSettings> SettingsFinder(
		TEXT("/Game/CoreSystems/Faction/DA_SettlementSettings"));
	if (SettingsFinder.Succeeded())
		Settings = SettingsFinder.Object;
}

void ASettlement::BeginPlay()
{
	Super::BeginPlay();
	GetWorld()->GetGameState<AGS_Ingame>()->LoadingManager->IncrementReplicationCount();
}

void ASettlement::S_Init(ATile* SpawnTile, UPopulationSettings* InPopulationSettings)
{
	PopulationSettings = InPopulationSettings;
	const FGameResources& StartingResources = Affiliation == EAffiliation::Enemy
		                                          ? Settings->C_StartingResources
		                                          : Settings->N_StartingResources;
	const TArray<UBuildingSettings*>& StartingBuildings = Affiliation == EAffiliation::Enemy
		                                                      ? Settings->C_StartingBuildings
		                                                      : Settings->N_StartingBuildings;
	GameplayTags = Affiliation == EAffiliation::Enemy
		               ? Settings->C_GameplayTags
		               : Settings->N_GameplayTags;
	S_AddResources(StartingResources);
	SpawnTile->TryBuild(StartingBuildings[0], this);
	SpawnTile->GetBuilding()->FinishConstruction();
	for (int32 i = 1; i < StartingBuildings.Num(); ++i)
	{
		if (BorderingUnclaimedTiles.Num() <= 0) break;
		ATile* Tile = BorderingUnclaimedTiles[FMath::RandRange(0, BorderingUnclaimedTiles.Num() - 1)];
		Tile->TryBuild(StartingBuildings[i], this);
		Tile->GetBuilding()->FinishConstruction();
	}
	for (ATile* Tile : ClaimedTiles)
	{
		Tile->GetBuilding()->Population->S_ChangeSize(100);
	}
}

void ASettlement::EnableTick()
{
	SetActorTickEnabled(true);
}

void ASettlement::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	UpdateLastMinuteResources();
	Resources.Food -= Population->GetSize() * Settings->PopEatingPerSecond * DeltaSeconds;

	if(Resources.Food < 0)
		PopulationSettings->IsStarving = true;
	else
		PopulationSettings->IsStarving = false;
		
	if (HasAuthority())
	{
		if (Resources.Food < Settings->StarvingThreshold)
		{
			int32 Count = Resources.Food / Settings->StarvingThreshold;
			Resources.Food += Count * Settings->StarvingThreshold * -1;
			for (int32 i = 0; i < Count; ++i)
			{
				Population->StarveRandomPop();
			}
		}
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
	Population->RegisterPop(Building->Population);
	ClaimedTiles.Add(Tile);
	RefreshBorderingUnclaimedTiles();
}

void ASettlement::OnBuildingRemoved(UBuilding* Building, ATile* Tile)
{
	Population->UnregisterPop(Building->Population);
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

void ASettlement::S_AddResources(const FGameResources Amount, const bool CountTowardsLastMinuteIncome)
{
	Resources += Amount;
	MARK_PROPERTY_DIRTY_FROM_NAME(ASettlement, Resources, this)
	ForceNetUpdate();
	if (CountTowardsLastMinuteIncome)
	{
		LastMinuteIncome += Amount;
		IncomeEvents.Enqueue(FIncomeEvent(Amount, GetWorld()->GetTimeSeconds()));
	}
}

void ASettlement::S_RemoveResources(const FGameResources Amount, const bool CountTowardsLastMinuteConsumption)
{
	Resources -= Amount;
	MARK_PROPERTY_DIRTY_FROM_NAME(ASettlement, Resources, this)
	ForceNetUpdate();
	if (CountTowardsLastMinuteConsumption)
	{
		LastMinuteConsumption += Amount;
		ConsumptionEvents.Enqueue(FIncomeEvent(Amount, GetWorld()->GetTimeSeconds()));
	}
}
