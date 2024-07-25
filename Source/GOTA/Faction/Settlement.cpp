// Fill out your copyright notice in the Description page of Project Settings.

#include "Settlement.h"

#include "Army.h"
#include "AI/NavigationSystemBase.h"
#include "GOTA/Core/GOTAGameState.h"
#include "GOTA/Core/LoadingManager.h"
#include "Net/UnrealNetwork.h"

void ASettlement::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	
	DOREPLIFETIME(ASettlement, ClaimColor);
	DOREPLIFETIME(ASettlement, PopulationSummary);
	DOREPLIFETIME(ASettlement, ProductionSummary);
	DOREPLIFETIME(ASettlement, CurrentBuildingProject);
	DOREPLIFETIME(ASettlement, Food);
	DOREPLIFETIME(ASettlement, Wood);
	DOREPLIFETIME(ASettlement, Stone);
	DOREPLIFETIME(ASettlement, Expansion);
	DOREPLIFETIME(ASettlement, PrimaryCulture);
	DOREPLIFETIME(ASettlement, Affiliation);
}

ASettlement::ASettlement()
{
	// Replication stuff
	bReplicates = true;
	bReplicateUsingRegisteredSubObjectList = true;
	Food = CreateDefaultSubobject<UGOTAAttribute>(TEXT("Food"));
	Wood = CreateDefaultSubobject<UGOTAAttribute>(TEXT("Wood"));
	Stone = CreateDefaultSubobject<UGOTAAttribute>(TEXT("Stone"));
	PopulationSummary = CreateDefaultSubobject<UPopulationSummary>(TEXT("Population"));
	ProductionSummary = CreateDefaultSubobject<UBuildingProductionSummary>(TEXT("Production"));
	Expansion = CreateDefaultSubobject<UGOTAAttributeLimited>(TEXT("Expansion"));
	CurrentBuildingProject = CreateDefaultSubobject<UBuildingProject>(TEXT("Current Building Project"));
}

void ASettlement::BeginPlay()
{
	Super::BeginPlay();
	
	// Get the GameState
	AGOTAGameState* GameState = GetWorld()->GetGameState<AGOTAGameState>();
	GameState->LoadingManager->IncrementReplicationCount();

	if(HasAuthority())
	{
		AddReplicatedSubObject(Food);
		AddReplicatedSubObject(Wood);
		AddReplicatedSubObject(Stone);
		AddReplicatedSubObject(PopulationSummary);
		AddReplicatedSubObject(ProductionSummary);
		AddReplicatedSubObject(Expansion);
		AddReplicatedSubObject(CurrentBuildingProject);
	}
}

void ASettlement::OnBuildingAdded(UBuilding* Building)
{
	if(Building)
	{
		UE_LOG(LogTemp, Log, TEXT("Building exist"));
	} else
	{
		UE_LOG(LogTemp, Log, TEXT("Building doesn't exist wtf"));
	}
	if(Building->Population)
	{
		UE_LOG(LogTemp, Log, TEXT("Building Population exist"));
	}
	PopulationSummary->RegisterPopulation(Building->Population);
	ProductionSummary->RegisterBuildingProduction(Building->Production);
}

void ASettlement::OnBuildingRemoved(UBuilding* Building)
{
}

bool ASettlement::SpawnArmy()
{
	// nowhere to spawn
	if(ClaimedTiles.IsEmpty()) return false;
	// random Tile that has no TileEntity
	ATile* SpawnLocation = ClaimedTiles[FMath::RandRange(0, ClaimedTiles.Num() - 1)];
	
	// spawn the army

	// evaluate how many pops to send
	// figure out which pops to send, remove them from the buildlings and add them to the army
	
	return true;
}

void ASettlement::SetCurrentBuildingProject(UBuildingProject* NewCurrentBuildingProject)
{
	if(CurrentBuildingProject)
	{
		RemoveReplicatedSubObject(CurrentBuildingProject);
	}
	CurrentBuildingProject = NewCurrentBuildingProject;
	OnCurrentBuildingProjectChanged.Broadcast();
	if(CurrentBuildingProject)
	{
		AddReplicatedSubObject(CurrentBuildingProject);
	}
}