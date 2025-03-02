// Fill out your copyright notice in the Description page of Project Settings.

#include "Army.h"

#include "Components/StateTreeComponent.h"
#include "GOTA/CoreSystems/Faction/Building/Building.h"
#include "GOTA/CoreSystems/Faction/Building/BuildingSettings.h"
#include "GOTA/CoreSystems/Faction/Settlement/Settlement.h"
#include "GOTA/CoreSystems/GameplayFramework/GS_Ingame.h"
#include "GOTA/CoreSystems/Tile/Tile.h"
#include "GOTA/CoreSystems/Tile/TileMap.h"
#include "Net/UnrealNetwork.h"
#include "Net/Core/PushModel/PushModel.h"

void AArmy::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	FDoRepLifetimeParams Params;
	Params.bIsPushBased = true;

	Params.Condition = COND_InitialOnly;
	Params.RepNotifyCondition = REPNOTIFY_Always;
	DOREPLIFETIME_WITH_PARAMS(AArmy, Building, Params);
	DOREPLIFETIME_WITH_PARAMS(AArmy, MovementRate, Params);
	DOREPLIFETIME_WITH_PARAMS(AArmy, RecruitRate, Params);
	DOREPLIFETIME_WITH_PARAMS(AArmy, Affiliation, Params);

	Params.Condition = COND_None;
	Params.RepNotifyCondition = REPNOTIFY_OnChanged;
	DOREPLIFETIME_WITH_PARAMS(AArmy, Progress, Params);
	DOREPLIFETIME_WITH_PARAMS(AArmy, Mode, Params);
	DOREPLIFETIME_WITH_PARAMS(AArmy, Status, Params);
	DOREPLIFETIME_WITH_PARAMS(AArmy, CurrentTile, Params);
	DOREPLIFETIME_WITH_PARAMS(AArmy, NetLocation, Params);
	DOREPLIFETIME_WITH_PARAMS(AArmy, CombatValues, Params);
	DOREPLIFETIME_WITH_PARAMS(AArmy, RavageSpeed, Params);
	DOREPLIFETIME_WITH_PARAMS(AArmy, GuardTile, Params);
	DOREPLIFETIME_WITH_PARAMS(AArmy, InterceptArmy, Params);
}

// ----------------------- LifeCycle -----------------------

AArmy::AArmy()
{
	bReplicates = true;
	bAlwaysRelevant = true;
	bReplicateUsingRegisteredSubObjectList = true;
	SetNetUpdateFrequency(1.0f);

	RootComponent = CreateDefaultSubobject<USceneComponent>("ROOT");
	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>("Static Mesh");
	MeshComponent->SetupAttachment(RootComponent);
	MeshComponent->SetRelativeScale3D(FVector(1, 1, 4));
	// I still don't understand why i need to set both: the ResponseChannel and CollisionEnabled
	// but this way it will only collide with ray casts, as intended
	MeshComponent->SetSimulatePhysics(false);
	MeshComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	MeshComponent->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	MeshComponent->SetCollisionResponseToChannel(ECC_Visibility, ECollisionResponse::ECR_Block);
	CombatValues = CreateDefaultSubobject<UCombatValues>("Combat Values");
	StateTree = CreateDefaultSubobject<UStateTreeComponent>("StateTree");
}


void AArmy::S_Init(UBuilding* InBuilding, ATile* SpawnTile)
{
	GameState = GetWorld()->GetGameState<AGS_Ingame>();
	Building = InBuilding;
	CurrentTile = SpawnTile;
	FVector NewLocation = FVector();
	SpawnTile->SetArmy(this, NewLocation);
	SetNetLocation(NewLocation);

	const UBuildingSettings* BuildingSettings = Building->GetSettings();
	RecruitRate = 100 / BuildingSettings->SecondsPerRecruitCycle;
	MovementRate = 100 / BuildingSettings->ArmyMoveTime;
	RavageSpeed = 100 / BuildingSettings->ArmyRavageTime;
	CombatValues->SetIndividualAttack(BuildingSettings->ArmyIndividualAttack);
	CombatValues->SetIndividualMaxHP(BuildingSettings->ArmyIndividualMaxHP);
	CombatValues->SetIndividualCount(BuildingSettings->ArmyIndividualCount);
	CombatValues->SetAttackSpeed(100 / BuildingSettings->ArmyAttackTime);
	CombatValues->OnDeath.AddDynamic(this, &AArmy::S_HandleDeath);
	Affiliation = Building->GetSettlement()->GetAffiliation();
	if (Affiliation == EAffiliation::Enemy)
		MeshComponent->SetStaticMesh(ColonyArmyMesh);
	else
		MeshComponent->SetStaticMesh(NativeArmyMesh);

	StateTree->StartLogic();
}

void AArmy::S_Tick(const float DeltaSeconds)
{
	C_Tick(DeltaSeconds);
	if (Progress >= 100)
	{
		if (GetStatus() == EArmyStatus::MovingToNextTile)
			S_MoveToNextTileOnPath();
		else if (GetStatus() == EArmyStatus::RecruitingFromTile)
			S_TakePopFromTile();
		else if (GetStatus() == EArmyStatus::Attacking)
			S_AttackEnemy();
		else if (GetStatus() == EArmyStatus::Ravaging)
			S_RavageEnemyBuilding();
		SetStatus(EArmyStatus::Idling);
		Progress = 0.0f;
		StateTree->SendStateTreeEvent(StateTreeCompletedTaskEventTag, FConstStructView(), FName(GetName()));
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
	else if (GetStatus() == EArmyStatus::Attacking)
	{
		Progress += CombatValues->GetAttackSpeed() * DeltaSeconds;
	}
	else if (GetStatus() == EArmyStatus::Ravaging)
	{
		Progress += RavageSpeed * DeltaSeconds;
	}
}

void AArmy::Delete()
{
	Destroy();
}

void AArmy::BeginDestroy()
{
	Super::BeginDestroy();
	S_HandleDeath();
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

void AArmy::SetMode(EArmyMode NewMode)
{
	if (Mode == NewMode) return;
	Mode = NewMode;
	MARK_PROPERTY_DIRTY_FROM_NAME(AArmy, Mode, this)
}

void AArmy::OnRep_Affiliation()
{
	if (Affiliation == EAffiliation::Enemy)
		MeshComponent->SetStaticMesh(ColonyArmyMesh);
	else
		MeshComponent->SetStaticMesh(NativeArmyMesh);
}

// ----------------------- Status -----------------------

void AArmy::SetStatus(EArmyStatus NewStatus)
{
	if (Status == NewStatus) return;
	Status = NewStatus;
	Progress = 0.0f;
	MARK_PROPERTY_DIRTY_FROM_NAME(AArmy, Status, this)
	MARK_PROPERTY_DIRTY_FROM_NAME(AArmy, Progress, this)
}

// ----------------------- Recruiting -----------------------

void AArmy::S_TakePopFromTile()
{
	if (CurrentTile->GetBuilding()->GetPopulation()->GetSize() <= 0)
		return;
	CombatValues->SetIndividualCount(CombatValues->GetIndividualCount() + 1);
	CurrentTile->GetBuilding()->GetPopulation()->S_DecreaseSize(1);
}

bool AArmy::IsTileValidForRecruiting(const ATile* Tile) const
{
	return Tile &&
		Tile->GetBuilding() &&
		!Tile->GetBuilding()->GetIsUnderConstruction() &&
		Tile->GetBuilding()->GetPopulation()->GetSize() == Tile->GetBuilding()->GetPopulation()->GetMaxSize() &&
		Tile->GetClaimant() &&
		Tile->GetClaimant() == Building->GetSettlement();
}

void AArmy::S_StartRecruitFromTile()
{
	Progress = 0.f;
	SetStatus(EArmyStatus::RecruitingFromTile);
}

bool AArmy::IsCurrentTileValidForRecruiting() const
{
	return IsTileValidForRecruiting(CurrentTile.Get());
}

bool AArmy::TryFindPathToNearestRecruitable()
{
	bool HasValidTiles = false;
	if (!Building.IsValid()) return false;
	for (ATile* Tile : Building->GetSettlement()->ClaimedTiles)
	{
		if (IsTileValidForRecruiting(Tile))
		{
			HasValidTiles = true;
			break;
		}
	}
	if (!HasValidTiles) return false;
	Path = GameState->GetTileMap()->FindPathToNearestTile(CurrentTile.Get(), EEntityType::Army,
	                                                      [this](const ATile* Tile)
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

void AArmy::S_StartMoveToNextTileOnPath()
{
	Progress = 0.f;
	SetStatus(EArmyStatus::MovingToNextTile);
}

void AArmy::S_MoveToNextTileOnPath()
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
bool AArmy::HasEnemyOnNeighboringTile() const
{
	return GetNeighboringEnemyArmies().Num() > 0 || GetNeighboringEnemyDefenseBuildings().Num() > 0;
}

bool AArmy::TryFindPathToNearestEnemy()
{
	if (!GameState->GetTileMap()) return false;
	Path = GameState->GetTileMap()->FindPathToNearestTile(CurrentTile.Get(), EEntityType::Army,
	                                                      [this](const ATile* Tile)
	                                                      {
		                                                      if (!Tile) return false;
		                                                      for (ATile* Neighbor : Tile->GetNeighbors())
		                                                      {
			                                                      if (Neighbor && Neighbor->GetArmy() && Neighbor->
				                                                      GetArmy()->GetAffiliation() != GetAffiliation())
				                                                      return true;
		                                                      }
		                                                      return false;
	                                                      });
	return !Path.IsEmpty();
}

bool AArmy::TryFindPathToNearestEnemyUnprotectedNormalBuilding()
{
	if (!GameState->GetTileMap()) return false;
	Path = GameState->GetTileMap()->FindPathToNearestTile(CurrentTile.Get(), EEntityType::Army,
	                                                      [this](const ATile* Tile)
	                                                      {
		                                                      return Tile &&
			                                                      Tile->GetClaimant() &&
			                                                      Tile->GetClaimant()->GetAffiliation() !=
			                                                      GetAffiliation() &&
			                                                      Tile->GetBuilding() &&
			                                                      !Tile->GetBuilding()->IsProtected();
	                                                      });
	return !Path.IsEmpty();
}

bool AArmy::TryFindPathToNearestEnemyDefenseBuilding()
{
	if (!GameState->GetTileMap()) return false;
	Path = GameState->GetTileMap()->FindPathToNearestTile(CurrentTile.Get(), EEntityType::Army,
	                                                      [this](const ATile* Tile)
	                                                      {
		                                                      if (!Tile) return false;
		                                                      for (ATile* Neighbor : Tile->GetNeighbors())
		                                                      {
			                                                      if (Neighbor &&
				                                                      Neighbor->GetClaimant() &&
				                                                      Neighbor->GetClaimant()->GetAffiliation() !=
				                                                      GetAffiliation() &&
				                                                      Neighbor->GetBuilding() &&
				                                                      Neighbor->GetBuilding()->GetSettings()->
				                                                      bDefenseEnabled)
			                                                      {
				                                                      return true;
			                                                      }
		                                                      }
		                                                      return false;
	                                                      });
	return !Path.IsEmpty();
}

bool AArmy::HasEnemyInGarrisonModeRange() const
{
	ATile* EnemyOnTile = GameState->GetTileMap()->FindNearestTileInRange(
		CurrentTile.Get(), GarrisonModeInterceptingRange,
		[this](const ATile* Tile)
		{
			return Tile && Tile->GetArmy() && Tile->GetArmy()
			                                      ->GetAffiliation() != GetAffiliation();
		});
	return EnemyOnTile != nullptr;
}

void AArmy::S_StartAttacking()
{
	Progress = 0.f;
	SetStatus(EArmyStatus::Attacking);
}

TArray<AArmy*> AArmy::GetNeighboringEnemyArmies() const
{
	TArray<AArmy*> NeighboringArmies;
	if (!CurrentTile.IsValid()) return NeighboringArmies;
	for (const ATile* Neighbor : CurrentTile->GetNeighbors())
	{
		if (Neighbor &&
			Neighbor->GetArmy() &&
			Neighbor->GetArmy()->GetAffiliation() != GetAffiliation())
		{
			NeighboringArmies.Add(Neighbor->GetArmy());
		}
	}
	return NeighboringArmies;
}

TArray<UBuilding*> AArmy::GetNeighboringEnemyDefenseBuildings() const
{
	TArray<UBuilding*> NeighboringDefenseBuildings;
	if (!CurrentTile.IsValid()) return NeighboringDefenseBuildings;
	for (const ATile* Neighbor : CurrentTile->GetNeighbors())
	{
		if (Neighbor &&
			Neighbor->GetClaimant() &&
			Neighbor->GetClaimant()->GetAffiliation() != GetAffiliation() &&
			Neighbor->GetBuilding() &&
			Neighbor->GetBuilding()->GetSettings()->bDefenseEnabled)
		{
			NeighboringDefenseBuildings.Add(Neighbor->GetBuilding());
		}
	}
	return NeighboringDefenseBuildings;
}

void AArmy::S_ArmyTakeDamage(int32 Damage)
{
	CombatValues->SetCurrentTotalHP(CombatValues->GetCurrentTotalHP() - Damage);
}

void AArmy::S_AttackEnemy()
{
	// choose enemy randomly
	TArray<AArmy*> AttackableArmies = GetNeighboringEnemyArmies();
	if (AttackableArmies.Num() > 0)
	{
		const int32 RandomIndex = FMath::RandRange(0, AttackableArmies.Num() - 1);
		AArmy* ChosenEnemy = AttackableArmies[RandomIndex];
		// inflict damage
		ChosenEnemy->S_ArmyTakeDamage(CombatValues->GetAttack());
	}
	TArray<UBuilding*> AttackableDefenseBuildings = GetNeighboringEnemyDefenseBuildings();
	if (AttackableDefenseBuildings.Num() > 0)
	{
		const int32 RandomIndex = FMath::RandRange(0, AttackableDefenseBuildings.Num() - 1);
		UBuilding* ChosenEnemy = AttackableDefenseBuildings[RandomIndex];
		// inflict damage
		ChosenEnemy->S_BuildingDefenseTakeDamage(CombatValues->GetAttack());
	}
}

void AArmy::S_HandleDeath()
{
	if (!CurrentTile.IsValid()) return;
	CurrentTile->RemoveArmy();
	Destroy();
}

// -----------------Ravaging------------------------

bool AArmy::IsOnEnemyBuilding() const
{
	return CurrentTile.IsValid() &&
		CurrentTile->GetBuilding() &&
		CurrentTile->GetClaimant() &&
		CurrentTile->GetClaimant()->GetAffiliation() != Affiliation;
}

bool AArmy::IsBuildingProtected() const
{
	return CurrentTile.IsValid() &&
		CurrentTile->GetBuilding() &&
		CurrentTile->GetBuilding()->IsProtected();
}

void AArmy::S_StartRavagingEnemyBuilding()
{
	Progress = 0.f;
	SetStatus(EArmyStatus::Ravaging);
}

void AArmy::S_RavageEnemyBuilding()
{
	if (!IsOnEnemyBuilding() || IsBuildingProtected()) return;
	if (CurrentTile->GetBuilding()->GetPopulation()->GetSize() > 0)
	{
		CurrentTile->GetBuilding()->GetPopulation()->S_DecreaseSize(1);
	}
	else
	{
		CurrentTile->S_Unbuild();
	}
}

// -----------------Guarding------------------------

bool AArmy::TryFindPathToGuardTile()
{
	if (!GameState->GetTileMap()) return false;
	Path = GameState->GetTileMap()->FindPathToNearestTile(CurrentTile.Get(), EEntityType::Army,
	                                                      [this](const ATile* Tile)
	                                                      {
		                                                      return Tile && Tile == GuardTile;
	                                                      });
	return !Path.IsEmpty();
}

bool AArmy::TryFindPathToNearestEnemyToGuardTile()
{
	if (!GameState->GetTileMap()) return false;
	Path = GameState->GetTileMap()->FindPathToNearestTile(GuardTile.Get(), EEntityType::Army, [this](const ATile* Tile)
	{
		if (!Tile) return false;
		for (ATile* Neighbor : Tile->GetNeighbors())
		{
			if (Neighbor && Neighbor->GetArmy() && Neighbor->GetArmy()->GetAffiliation() != Affiliation)
				return true;
		}
		return false;
	});
	return !Path.IsEmpty();
}

bool AArmy::HasEnemyInGuardTileRange()
{
	ATile* EnemyOnTile = GameState->GetTileMap()->FindNearestTileInRange(
		GuardTile.Get(), GuardModeInterceptingRange,
		[this](const ATile* Tile)
		{
			return Tile && Tile->GetArmy() && Tile->GetArmy()->GetAffiliation() != Affiliation;
		});
	return EnemyOnTile != nullptr;
}

// -----------------Intercepting------------------------

bool AArmy::TryFindPathToInterceptArmy()
{
	if (!GameState->GetTileMap()) return false;
	Path = GameState->GetTileMap()->FindPathToNearestTile(CurrentTile.Get(), EEntityType::Army,
	                                                      [this](const ATile* Tile)
	                                                      {
		                                                      if (!Tile) return false;
		                                                      for (ATile* Neighbor : Tile->GetNeighbors())
		                                                      {
			                                                      if (Neighbor && Neighbor->GetArmy() && Neighbor->
				                                                      GetArmy() == InterceptArmy)
				                                                      return true;
		                                                      }
		                                                      return false;
	                                                      });
	return !Path.IsEmpty();
}
