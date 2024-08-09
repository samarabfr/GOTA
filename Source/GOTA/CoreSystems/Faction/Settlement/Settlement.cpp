// Fill out your copyright notice in the Description page of Project Settings.

#include "Settlement.h"

#include "Components/InstancedStaticMeshComponent.h"
#include "GOTA/CoreSystems/Faction/Attribute/GOTAAttribute.h"
#include "GOTA/CoreSystems/Faction/Attribute/GOTAAttributeLimited.h"
#include "GOTA/CoreSystems/Faction/Building/Building.h"
#include "GOTA/CoreSystems/GameplayFramework/GS_Ingame.h"
#include "GOTA/CoreSystems/GameplayFramework/LoadingManager.h"
#include "Net/UnrealNetwork.h"

void ASettlement::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ASettlement, ClaimColor);
	DOREPLIFETIME(ASettlement, PopulationSummary);
	DOREPLIFETIME(ASettlement, BuildingSummary);
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
	RootComponent = CreateDefaultSubobject<USceneComponent>("ROOT");
	ISM_ClaimWalls = CreateDefaultSubobject<UInstancedStaticMeshComponent>("Claim Walls");
	ISM_ClaimWalls->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	ISM_ClaimWalls->SetupAttachment(RootComponent);
	ISM_ClaimWallsRiver = CreateDefaultSubobject<UInstancedStaticMeshComponent>("Claim Walls River");
	ISM_ClaimWallsRiver->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	ISM_ClaimWallsRiver->SetupAttachment(RootComponent);

	// Replication stuff
	bReplicates = true;
	bReplicateUsingRegisteredSubObjectList = true;
	Food = CreateDefaultSubobject<UGOTAAttribute>(TEXT("Food"));
	Wood = CreateDefaultSubobject<UGOTAAttribute>(TEXT("Wood"));
	Stone = CreateDefaultSubobject<UGOTAAttribute>(TEXT("Stone"));
	PopulationSummary = CreateDefaultSubobject<UPopulationSummary>(TEXT("Population"));
	BuildingSummary = CreateDefaultSubobject<UBuildingSummary>(TEXT("Production"));
	Expansion = CreateDefaultSubobject<UGOTAAttributeLimited>(TEXT("Expansion"));
	CurrentBuildingProject = CreateDefaultSubobject<UBuildingProject>(TEXT("Current Building Project"));
}

void ASettlement::OnRep_ClaimColor()
{
	ISM_ClaimWalls->SetStaticMesh(ClaimMesh);
	UMaterialInstanceDynamic* DynMaterial = UMaterialInstanceDynamic::Create(ClaimMaterial, this);
	DynMaterial->SetVectorParameterValue(EName::Color, ClaimColor);
	ISM_ClaimWalls->SetMaterialByName(FName("Flag"), DynMaterial);
}

void ASettlement::BeginPlay()
{
	Super::BeginPlay();

	// Get the GameState
	AGS_Ingame* GameState = GetWorld()->GetGameState<AGS_Ingame>();
	GameState->LoadingManager->IncrementReplicationCount();

	if (HasAuthority())
	{
		ISM_ClaimWalls->SetStaticMesh(ClaimMesh);
		ClaimColor = FLinearColor(FMath::FRand(), FMath::FRand(), FMath::FRand());
		UMaterialInstanceDynamic* DynMaterial = UMaterialInstanceDynamic::Create(ClaimMaterial, this);
		DynMaterial->SetVectorParameterValue(EName::Color, ClaimColor);
		ISM_ClaimWalls->SetMaterialByName(FName("Flag"), DynMaterial);

		AddReplicatedSubObject(Food);
		AddReplicatedSubObject(Wood);
		AddReplicatedSubObject(Stone);
		AddReplicatedSubObject(PopulationSummary);
		AddReplicatedSubObject(BuildingSummary);
		AddReplicatedSubObject(Expansion);
		AddReplicatedSubObject(CurrentBuildingProject);
	}
}

FPrimitiveInstanceId ASettlement::AddClaimMeshInstance(FTransform& Transform)
{
	return ISM_ClaimWalls->AddInstanceById(Transform);
}

void ASettlement::RemoveClaimMeshInstance(FPrimitiveInstanceId InstanceId)
{
	ISM_ClaimWalls->RemoveInstanceById(InstanceId);
}

bool ASettlement::ClaimRandomTile()
{
	// All Neighboring tiles that have 3 or more neighbors already claimed by this settlement
	TArray<ATile*> BorderingTilesHighPrio;
	// All other claimable neighbors
	TArray<ATile*> BorderingTilesLowPrio;
	// Fill Bordering Arrays
	for (ATile* ClaimedTile : ClaimedTiles)
	{
		for (int32 NeighborIndex = 0; NeighborIndex < 6; ++NeighborIndex)
		{
			// ignore this tile if its null or not claimable
			if (!ClaimedTile->Neighbors[NeighborIndex] || !ClaimedTile->Neighbors[NeighborIndex]->IsClaimable())
				continue;
			// check how many of this neighbor neighbors are claimed by this settlement
			int32 NeighborClaimedNeighbors = 0;
			for (int32 i = 0; i < 6; ++i)
			{
				if (ClaimedTile->Neighbors[NeighborIndex]->Neighbors[i]
					&& ClaimedTile->Neighbors[NeighborIndex]->Neighbors[i]->GetClaimant() == this)
				{
					NeighborClaimedNeighbors++;
				}
			}
			if (NeighborClaimedNeighbors >= 3)
			{
				BorderingTilesHighPrio.Add(ClaimedTile->Neighbors[NeighborIndex]);
			}
			else
			{
				BorderingTilesLowPrio.Add(ClaimedTile->Neighbors[NeighborIndex]);
			}
		}
	}
	if (!BorderingTilesHighPrio.IsEmpty())
	{
		// we have at least one High Prio claimable neighbor lets go
		ClaimTile(BorderingTilesHighPrio[FMath::RandRange(0, BorderingTilesHighPrio.Num() - 1)]);
		return true;
	}
	if (!BorderingTilesLowPrio.IsEmpty())
	{
		// well we at least have a low prio tile to claim, good enough
		ClaimTile(BorderingTilesLowPrio[FMath::RandRange(0, BorderingTilesLowPrio.Num() - 1)]);
		return true;
	}
	return false;
}

void ASettlement::OnBuildingAdded(UBuilding* Building)
{
	PopulationSummary->RegisterPopulationContainer(Building->PopContainer);
	BuildingSummary->RegisterBuildingProduction(Building);
}

void ASettlement::OnBuildingRemoved(UBuilding* Building)
{
	BuildingSummary->UnregisterBuildingProduction(Building);
}

bool ASettlement::SpawnArmy()
{
	// nowhere to spawn
	if (ClaimedTiles.IsEmpty()) return false;
	// random Tile that has no TileEntity
	ATile* SpawnLocation = ClaimedTiles[FMath::RandRange(0, ClaimedTiles.Num() - 1)];

	// spawn the army

	// evaluate how many pops to send
	// figure out which pops to send, remove them from the buildings and add them to the army

	return true;
}


void ASettlement::SetCurrentBuildingProject(UBuildingProject* NewCurrentBuildingProject)
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
