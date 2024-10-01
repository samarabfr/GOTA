// Fill out your copyright notice in the Description page of Project Settings.

#include "Army.h"

#include "ArmySettings.h"
#include "GOTA/CoreSystems/Faction/Building/Building.h"
#include "GOTA/CoreSystems/Faction/Settlement/Settlement.h"
#include "GOTA/CoreSystems/GameplayFramework/GS_Ingame.h"
#include "GOTA/CoreSystems/Tile/Tile.h"

AArmy::AArmy()
{
	bReplicates = true;
	bAlwaysRelevant = true;
	bReplicateUsingRegisteredSubObjectList = true;
	NetUpdateFrequency = 1.0f;

	ConstructorHelpers::FObjectFinder<UArmySettings> DataAsset(
		TEXT("/Game/CoreSystems/Entity/DA_Army"));
	Settings = DataAsset.Object;

	RootComponent = CreateDefaultSubobject<USceneComponent>("ROOT");
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>("Static Mesh");
	Mesh->SetupAttachment(RootComponent);
	Mesh->SetRelativeScale3D(FVector(1, 1, 4));
	Mesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void AArmy::Init(ASettlement* InSettlement, ATile* SpawnTile, float InRecruitRate, float InMovementRate, int32 InSize)
{
	GameState = GetWorld()->GetGameState<AGS_Ingame>();
	Settlement = InSettlement;
	CurrentTile = SpawnTile;
	SpawnTile->SetArmy(this);
	RecruitRate = InRecruitRate;
	MovementRate = InMovementRate;
	Size = InSize;
	Affiliation = Settlement->Affiliation;
	if(Affiliation == EAffiliation::Enemy)
		Mesh->SetStaticMesh(Settings->ColonyArmyMesh);
	else
		Mesh->SetStaticMesh(Settings->NativeArmyMesh);
}

void AArmy::GOTATick(float DeltaSeconds)
{
	ValidateStatus();
	if (GetStatus() == EArmyStatus::Moving)
	{
		Progress += MovementRate * DeltaSeconds;
	}
	else if (GetStatus() == EArmyStatus::Recruiting)
	{
		Progress += RecruitRate * DeltaSeconds;
	}
	if (Progress >= 100)
	{
		if (GetStatus() == EArmyStatus::Moving)
			Move();
		else if (GetStatus() == EArmyStatus::Recruiting)
			Recruit();
		Progress = 0.0f;
		ValidateStatus();
	}
}

EAffiliation AArmy::GetAffiliation() const
{
	return Affiliation;
}

void AArmy::ValidateStatus()
{
	CheckForCombat();
	if (Status == EArmyStatus::Fighting) return;
	if (Status == EArmyStatus::Idle)
	{
		if (IsTileValidForRecruiting(CurrentTile))
			SetStatus(EArmyStatus::Recruiting);
		else if (TryFindPath())
			SetStatus(EArmyStatus::Moving);
	}
	else if (GetStatus() == EArmyStatus::Moving)
	{
		if ((Path.IsEmpty() || !Path[Path.Num() - 1]->AcceptsArmy()) && !TryFindPath())
			SetStatus(EArmyStatus::Idle);
	}
	else if (GetStatus() == EArmyStatus::Recruiting)
	{
		if (!IsTileValidForRecruiting(CurrentTile))
		{
			if (TryFindPath())
				SetStatus(EArmyStatus::Moving);
			else
				SetStatus(EArmyStatus::Idle);
		}
	}
}

bool AArmy::TryFindPath()
{
	bool HasValidTiles = false;
	for (ATile* Tile : Settlement->ClaimedTiles)
	{
		if (IsTileValidForRecruiting(Tile))
		{
			HasValidTiles = true;
			break;
		}
	}
	if (!HasValidTiles) return false;
	Path = GameState->TileMap->FindPathToNearestTile(CurrentTile, EEntityType::Civilian, [this](const ATile* Tile)
	{
		return IsTileValidForRecruiting(Tile);
	});
	return !Path.IsEmpty();
}

bool AArmy::IsTileValidForRecruiting(const ATile* Tile) const
{
	return Tile->Building
		&& Tile->Building->Population->GetSize() == Tile->Building->Population->GetMaxSize()
		&& Tile->GetClaimant()
		&& Tile->GetClaimant() == Settlement;
}

void AArmy::SetStatus(EArmyStatus NewStatus)
{
	if (Status == NewStatus) return;
	Status = NewStatus;
	Progress = 0.0f;
}

void AArmy::Move()
{
	ATile* NewCurrent = nullptr;
	if (!Path.IsEmpty())
	{
		NewCurrent = Path.Pop();
	}
	if (!NewCurrent) return;
	CurrentTile->RemoveArmy();
	NewCurrent->SetArmy(this);
	CurrentTile = NewCurrent;
}

void AArmy::Recruit()
{
	++Size;
	CurrentTile->Building->Population->DecreaseSize(1);
}

void AArmy::CheckForCombat()
{
	if (Status == EArmyStatus::Fighting) return;
	for (ATile* Neighbor : CurrentTile->Neighbors)
	{
		if (Neighbor &&
			Neighbor->GetArmy() &&
			Neighbor->GetArmy()->Affiliation != Affiliation)
		{
			if (Neighbor->GetArmy()->Status != EArmyStatus::Fighting)
			{
				InitializeCombat(Neighbor->GetArmy());
			}
			else
			{
				JoinCombat(Neighbor->GetArmy());
			}
		}
	}
}

void AArmy::InitializeCombat(AArmy* Enemy)
{
	Status = EArmyStatus::Fighting;
	Enemy->ChallengeToCombat();
}

void AArmy::JoinCombat(AArmy* Enemy)
{
	Status = EArmyStatus::Fighting;
}

void AArmy::ChallengeToCombat()
{
	Status = EArmyStatus::Fighting;
}
