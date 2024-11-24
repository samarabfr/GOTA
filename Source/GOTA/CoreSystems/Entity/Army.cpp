// Fill out your copyright notice in the Description page of Project Settings.

#include "Army.h"

#include "ArmySettings.h"
#include "GOTA/CoreSystems/Faction/Building/Building.h"
#include "GOTA/CoreSystems/Faction/Building/BuildingSettings.h"
#include "GOTA/CoreSystems/Faction/Settlement/Settlement.h"
#include "GOTA/CoreSystems/GameplayFramework/GS_Ingame.h"
#include "GOTA/CoreSystems/Tile/Tile.h"
#include "GOTA/CoreSystems/Tile/TileMap.h"
#include "Net/UnrealNetwork.h"
#include "Net/Core/PushModel/PushModel.h"
#include "StateTree/StateTreeComponentArmy.h"

void AArmy::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	FDoRepLifetimeParams Params;
	Params.bIsPushBased = true;

	Params.Condition = COND_InitialOnly;
	Params.RepNotifyCondition = REPNOTIFY_Always;
	DOREPLIFETIME_WITH_PARAMS(AArmy, Building, Params);
	DOREPLIFETIME_WITH_PARAMS(AArmy, Settings, Params);
	DOREPLIFETIME_WITH_PARAMS(AArmy, MovementRate, Params);
	DOREPLIFETIME_WITH_PARAMS(AArmy, Affiliation, Params);

	Params.Condition = COND_None;
	Params.RepNotifyCondition = REPNOTIFY_OnChanged;
	DOREPLIFETIME_WITH_PARAMS(AArmy, Progress, Params);
	DOREPLIFETIME_WITH_PARAMS(AArmy, Mode, Params);
	DOREPLIFETIME_WITH_PARAMS(AArmy, Status, Params);
	DOREPLIFETIME_WITH_PARAMS(AArmy, CurrentTile, Params);
	DOREPLIFETIME_WITH_PARAMS(AArmy, NetLocation, Params);
	DOREPLIFETIME_WITH_PARAMS(AArmy, CombatValues, Params);
}

// ----------------------- LifeCycle -----------------------

AArmy::AArmy()
{
	ConstructorHelpers::FObjectFinder<UArmySettings> SettingsFinder(
		TEXT("/Game/CoreSystems/Entity/DA_Army"));
	Settings = SettingsFinder.Object;

	bReplicates = true;
	bAlwaysRelevant = true;
	bReplicateUsingRegisteredSubObjectList = true;
	NetUpdateFrequency = 1.0f;

	RootComponent = CreateDefaultSubobject<USceneComponent>("ROOT");
	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>("Static Mesh");
	StateTree = CreateDefaultSubobject<UStateTreeComponentArmy>("StateTree");
	CombatValues = CreateDefaultSubobject<UCombatValues>("Combat Values");
	MeshComponent->SetupAttachment(RootComponent);
	MeshComponent->SetRelativeScale3D(FVector(1, 1, 4));
	// I still don't understand why i need to set both: the ResponseChannel and CollisionEnabled
	// but this way it will only collide with ray casts, as intended
	MeshComponent->SetSimulatePhysics(false);
	MeshComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	MeshComponent->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	MeshComponent->SetCollisionResponseToChannel(ECC_Visibility, ECollisionResponse::ECR_Block);
}


void AArmy::S_Init(UBuilding* InBuilding, ATile* SpawnTile)
{
	GameState = GetWorld()->GetGameState<AGS_Ingame>();
	Building = InBuilding;
	CurrentTile = SpawnTile;
	FVector NewLocation = FVector();
	SpawnTile->SetArmy(this, NewLocation);
	SetNetLocation(NewLocation);

	const UBuildingSettings* BuildingSettings = Building->Settings;
	RecruitRate = 100 / BuildingSettings->SecondsPerRecruitCycle;
	MovementRate = 100 / BuildingSettings->ArmySecondsPerMove;
	CombatValues->SetIndividualAttack(BuildingSettings->ArmyIndividualAttack);
	CombatValues->SetIndividualHP(BuildingSettings->ArmyIndividualHP);
	CombatValues->SetIndividualCount(BuildingSettings->ArmyIndividualCount);
	CombatValues->SetAttackSpeed(100 / BuildingSettings->ArmySecondsPerAttack);
	Affiliation = Building->Settlement->Affiliation;
	if (Affiliation == EAffiliation::Enemy)
		MeshComponent->SetStaticMesh(Settings->ColonyArmyMesh);
	else
		MeshComponent->SetStaticMesh(Settings->NativeArmyMesh);
}

void AArmy::S_Tick(const float DeltaSeconds)
{
	C_Tick(DeltaSeconds);
	if (Progress >= 100)
	{
		if (GetStatus() == EArmyStatus::MovingToNextTile)
			MoveToNextTileOnPath();
		else if (GetStatus() == EArmyStatus::RecruitingFromTile)
			TakePopFromTile();
		SetStatus(EArmyStatus::Idling);
		Progress = 0.0f;
		StateTree->SendStateTreeEvent(Settings->StateTreeCompletedTaskEventTag, FConstStructView(), FName(GetName()));
		MARK_PROPERTY_DIRTY_FROM_NAME(AArmy, Progress, this)
	}
}

void AArmy::C_Tick(const float DeltaSeconds)
{
	if (GetStatus() == EArmyStatus::MovingToNextTile)
	{
		Progress += MovementRate * DeltaSeconds;
	}
	else if (GetStatus() == EArmyStatus::RecruitingFromTile)
	{
		Progress += RecruitRate * DeltaSeconds;
	}
	else if (GetStatus() == EArmyStatus::RecruitingFromTile)
	{
		Progress += CombatValues->GetAttackSpeed() * DeltaSeconds;
	}
}

void AArmy::BeginDestroy()
{
	Super::BeginDestroy();
}

// -----------------------  -----------------------

EAffiliation AArmy::GetAffiliation() const
{
	return Affiliation;
}

EArmyMode AArmy::GetMode() const
{
	return Mode;
}

void AArmy::SetArmyMode(EArmyMode NewMode)
{
	if (Mode == NewMode) return;
	Mode = NewMode;
	MARK_PROPERTY_DIRTY_FROM_NAME(AArmy, Mode, this)
}

void AArmy::OnRep_Affiliation()
{
	if (Affiliation == EAffiliation::Enemy)
		MeshComponent->SetStaticMesh(Settings->ColonyArmyMesh);
	else
		MeshComponent->SetStaticMesh(Settings->NativeArmyMesh);
}

// ----------------------- Status -----------------------

void AArmy::SetStatus(EArmyStatus NewStatus)
{
	if (Status == NewStatus) return;
	Status = NewStatus;
	MARK_PROPERTY_DIRTY_FROM_NAME(AArmy, Status, this)
}

// ----------------------- Recruiting -----------------------

void AArmy::TakePopFromTile()
{
	if (CurrentTile->GetBuilding()->Population->GetSize() <= 0)
		return;
	CombatValues->SetIndividualCount(CombatValues->GetIndividualCount() + 1);
	CurrentTile->GetBuilding()->Population->S_DecreaseSize(1);
}

bool AArmy::IsTileValidForRecruiting(const ATile* Tile) const
{
	return Tile
		&& Tile->GetBuilding()
		&& Tile->GetBuilding()->Population->GetSize() == Tile->GetBuilding()->Population->GetMaxSize()
		&& Tile->GetClaimant()
		&& Tile->GetClaimant() == Building->Settlement;
}

void AArmy::StartRecruitFromTile()
{
	Progress = 0.f;
	SetStatus(EArmyStatus::RecruitingFromTile);
}

bool AArmy::IsCurrentTileValidForRecruiting() const
{
	return IsTileValidForRecruiting(CurrentTile);
}

bool AArmy::TryFindPathToNearestRecruitable()
{
	bool HasValidTiles = false;
	if (!Building) return false;
	for (ATile* Tile : Building->Settlement->ClaimedTiles)
	{
		if (IsTileValidForRecruiting(Tile))
		{
			HasValidTiles = true;
			break;
		}
	}
	if (!HasValidTiles) return false;
	Path = GameState->GetTileMap()->FindPathToNearestTile(CurrentTile, EEntityType::Army, [this](const ATile* Tile)
	{
		return IsTileValidForRecruiting(Tile);
	});
	return !Path.IsEmpty();
}

// ----------------- Moving ------------------------

void AArmy::OnRep_NetLocation()
{
	SetActorLocation(NetLocation);
}

void AArmy::SetNetLocation(const FVector& NewNetLocation)
{
	SetActorLocation(NewNetLocation);
	NetLocation = NewNetLocation;
	MARK_PROPERTY_DIRTY_FROM_NAME(AArmy, NetLocation, this)
}

bool AArmy::IsPathValid()
{
	return !Path.IsEmpty() && Path[Path.Num() - 1]->AcceptsArmy();
}

void AArmy::StartMoveToNextTileOnPath()
{
	Progress = 0.f;
	SetStatus(EArmyStatus::MovingToNextTile);
}

void AArmy::MoveToNextTileOnPath()
{
	ATile* NewCurrent = nullptr;
	if (IsPathValid())
	{
		NewCurrent = Path.Pop();
	}
	if (!NewCurrent) return;
	CurrentTile->RemoveArmy();
	FVector NewLocation = FVector();
	NewCurrent->SetArmy(this, NewLocation);
	SetNetLocation(NewLocation);
	CurrentTile = NewCurrent;
	MARK_PROPERTY_DIRTY_FROM_NAME(AArmy, CurrentTile, this)
}

// ----------------- Combat ------------------------

UCombatValues* AArmy::GetCombatValues() const
{
	return CombatValues;
}

bool AArmy::HasEnemyOnNeighboringTile() const
{
	if (!CurrentTile) return false;
	for (ATile* Neighbor : CurrentTile->Neighbors)
	{
		if (Neighbor &&
			Neighbor->GetArmy() &&
			Neighbor->GetArmy()->GetAffiliation() != GetAffiliation())
		{
			return true;
		}
	}
	return false;
}

bool AArmy::HasCombatOnNeighboringTile() const
{
	if (!CurrentTile) return false;
	for (ATile* Neighbor : CurrentTile->Neighbors)
	{
		if (Neighbor &&
			Neighbor->GetArmy() &&
			Neighbor->GetArmy()->GetAffiliation() != GetAffiliation())
		{
			return true;
		}
	}
	return false;
}

bool AArmy::TryFindPathToNearestEnemy()
{
	Path = GameState->GetTileMap()->FindPathToNearestTile(CurrentTile, EEntityType::Army, [this](const ATile* Tile)
	{
		if (!Tile) return false;
		for (ATile* Neighbor : Tile->Neighbors)
		{
			if (Neighbor && Neighbor->GetArmy() && Neighbor->GetArmy()->GetAffiliation() != GetAffiliation())
				return true;
		}
		return false;
	});
	return !Path.IsEmpty();
}

bool AArmy::TryFindPathToNearestEnemyBuilding()
{
	Path = GameState->GetTileMap()->FindPathToNearestTile(CurrentTile, EEntityType::Army, [this](const ATile* Tile)
	{
		return Tile && Tile->GetBuilding() && Tile->GetClaimant()
			&& Tile->GetClaimant()->Affiliation != GetAffiliation();
	});
	return !Path.IsEmpty();
}

bool AArmy::HasEnemyInGarrisonModeRange() const
{
	ATile* EnemyOnTile = GameState->GetTileMap()->FindNearestTileInRange(
		CurrentTile, Settings->GarrisonModeInterceptingRange,
		[this](const ATile* Tile)
		{
			return Tile && Tile->GetArmy() && Tile->GetArmy()
			                                      ->GetAffiliation() != GetAffiliation();
		});
	return EnemyOnTile != nullptr;
}

void AArmy::Fight()
{
	SetStatus(EArmyStatus::Fighting);
}
