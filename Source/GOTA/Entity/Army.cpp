// Fill out your copyright notice in the Description page of Project Settings.

#include "Army.h"

#include "GOTA/Tile/Building/Building.h"
#include "GOTA/Tile/Building/BuildingSettings.h"
#include "GOTA/Settlement/Settlement.h"
#include "GOTA/Utility/CombatValues.h"
#include "GOTA/GameplayFramework/GS_Ingame.h"
#include "GOTA/Tile/Tile.h"
#include "GOTA/Tile/Building/Population.h"
#include "GOTA/Tilemap/TileMap.h"
#include "Net/UnrealNetwork.h"
#include "Net/Core/PushModel/PushModel.h"

// ----------------------- Replication Setup -----------------------

void AArmy::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	FDoRepLifetimeParams Params;
	Params.bIsPushBased = true;

	Params.Condition = COND_InitialOnly;
	Params.RepNotifyCondition = REPNOTIFY_Always;
	DOREPLIFETIME_WITH_PARAMS(AArmy, RecruitRate, Params);

	Params.Condition = COND_None;
	Params.RepNotifyCondition = REPNOTIFY_OnChanged;
	DOREPLIFETIME_WITH_PARAMS(AArmy, Mode, Params);
	DOREPLIFETIME_WITH_PARAMS(AArmy, CombatValues, Params);
	DOREPLIFETIME_WITH_PARAMS(AArmy, RavageSpeed, Params);
	DOREPLIFETIME_WITH_PARAMS(AArmy, GuardTile, Params);
	DOREPLIFETIME_WITH_PARAMS(AArmy, InterceptArmy, Params);
}

// ----------------------- LifeCycle -----------------------

AArmy::AArmy()
{
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
}


void AArmy::S_Init(UBuilding* InBuilding, ATile* SpawnTile)
{
	Super::S_Init(InBuilding, SpawnTile);

	FVector NewLocation = FVector();
	SpawnTile->SetArmy(this, NewLocation);
	SetNetLocation(NewLocation);

	const UBuildingSettings* BuildingSettings = GetOriginBuilding()->GetSettings();
	RecruitRate = 100 / BuildingSettings->SecondsPerRecruitCycle;
	S_SetMovementRate(100 / BuildingSettings->ArmyMoveTime);
	RavageSpeed = 100 / BuildingSettings->ArmyRavageTime;
	GoAttackModeTimeLeft = BuildingSettings->ArmyGoAttackModeTime;
	CombatValues->S_Init(BuildingSettings->ArmyIndividualAttack,
	                     BuildingSettings->ArmyIndividualMaxHP,
	                     BuildingSettings->ArmyIndividualCount,
	                     100 / BuildingSettings->ArmyAttackTime);
	CombatValues->OnDeath.AddDynamic(this, &AArmy::Delete);
	RefreshMesh();
}

void AArmy::S_Tick(const float DeltaSeconds)
{
	Super::S_Tick(DeltaSeconds);
	if (GoAttackModeTimeLeft <= 0.0f)
	{
		GoAttackModeTimeLeft = 0.0f;
		SetMode(EArmyMode::AttackMode);
	}
	else
	{
		GoAttackModeTimeLeft -= DeltaSeconds;
	}
}

void AArmy::OnRep_Affiliation()
{
	Super::OnRep_Affiliation();
	RefreshMesh();
}

void AArmy::RefreshMesh()
{
	if (GetAffiliation() == EAffiliation::Enemy)
		MeshComponent->SetStaticMesh(ColonyArmyMesh);
	else
		MeshComponent->SetStaticMesh(NativeArmyMesh);
}

EEntityType AArmy::GetEntityType() const
{
	return EEntityType::Army;
}

// ----------------- Mode ------------------------

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

// ----------------------- Recruiting -----------------------

void AArmy::S_TakePopFromTile()
{
	if (GetCurrentTile()->GetBuilding()->GetPopulation()->GetSize() <= 0)
		return;
	CombatValues->S_SetIndividualCount(CombatValues->GetIndividualCount() + 1);
	GetCurrentTile()->GetBuilding()->GetPopulation()->S_DecreaseSize(1);
}

bool AArmy::IsTileValidForRecruiting(const ATile* Tile) const
{
	return Tile &&
		Tile->GetBuilding() &&
		!Tile->GetBuilding()->GetIsUnderConstruction() &&
		Tile->GetBuilding()->GetPopulation()->GetSize() == Tile->GetBuilding()->GetPopulation()->GetMaxSize() &&
		Tile->GetClaimant() &&
		Tile->GetClaimant() == GetOriginBuilding()->GetSettlement();
}

float AArmy::GetRecruitRate() const
{
	return RecruitRate;
}

bool AArmy::IsCurrentTileValidForRecruiting() const
{
	return IsTileValidForRecruiting(GetCurrentTile());
}

TArray<ATile*> AArmy::FindPathToNearestRecruitable() const
{
	bool HasValidTiles = false;
	if (!GetOriginBuilding()) return TArray<ATile*>();
	for (ATile* Tile : GetOriginBuilding()->GetSettlement()->ClaimedTiles)
	{
		if (IsTileValidForRecruiting(Tile))
		{
			HasValidTiles = true;
			break;
		}
	}
	if (!HasValidTiles) return TArray<ATile*>();
	TArray<ATile*> Origin;
	Origin.Add(GetCurrentTile());
	const TArray<ATile*> ResultPath = S_GetGameState()->GetTileMap()->FindPathToNearestTile(
		Origin, EEntityType::Army,
		[this](const ATile* Tile)
		{
			return IsTileValidForRecruiting(Tile);
		});
	return ResultPath;
}


// ----------------- Combat ------------------------
bool AArmy::HasEnemyOnNeighboringTile() const
{
	return GetNeighboringEnemyArmies().Num() > 0 || GetNeighboringEnemyDefenseBuildings().Num() > 0;
}

TArray<ATile*> AArmy::FindPathToNearestEnemy() const
{
	if (!S_GetGameState()->GetTileMap()) return TArray<ATile*>();
	TArray<ATile*> Origin;
	Origin.Add(GetCurrentTile());
	const TArray<ATile*> ResultPath = S_GetGameState()->GetTileMap()->FindPathToNearestTile(
		Origin, EEntityType::Army,
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
	return ResultPath;
}

TArray<ATile*> AArmy::FindPathToNearestEnemyUnprotectedNormalBuilding() const
{
	if (!S_GetGameState()->GetTileMap()) return TArray<ATile*>();
	TArray<ATile*> Origin;
	Origin.Add(GetCurrentTile());
	const TArray<ATile*> ResultPath = S_GetGameState()->GetTileMap()->FindPathToNearestTile(
		Origin, EEntityType::Army,
		[this](const ATile* Tile)
		{
			return Tile &&
				Tile->GetClaimant() &&
				Tile->GetClaimant()->GetAffiliation() !=
				GetAffiliation() &&
				Tile->GetBuilding() &&
				!Tile->GetBuilding()->IsProtected() &&
				!Tile->GetBuilding()->GetIsUnderConstruction();
		});
	return ResultPath;
}

TArray<ATile*> AArmy::FindPathToNearestEnemyDefenseBuilding() const
{
	if (!S_GetGameState()->GetTileMap()) return TArray<ATile*>();
	TArray<ATile*> Origin;
	Origin.Add(GetCurrentTile());
	const TArray<ATile*> ResultPath = S_GetGameState()->GetTileMap()->FindPathToNearestTile(
		Origin, EEntityType::Army,
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
					Neighbor->GetBuilding()->GetSettings()->bDefenseEnabled &&
					!Neighbor->GetBuilding()->GetIsUnderConstruction())
				{
					return true;
				}
			}
			return false;
		});
	return ResultPath;
}

bool AArmy::HasEnemyInGarrisonModeRange() const
{
	TArray<ATile*> Origin;
	Origin.Add(GetCurrentTile());
	ATile* EnemyOnTile = S_GetGameState()->GetTileMap()->FindNearestTileInRange(
		Origin, GarrisonModeInterceptingRange,
		EEntityType::None, [this](const ATile* Tile)
		{
			if (!Tile)
				return false;
			if (!Tile->GetArmy())
				return false;
			if (Tile->GetArmy()->GetAffiliation() == GetAffiliation())
				return false;
			return true;
		});
	return EnemyOnTile != nullptr;
}

UCombatValues* AArmy::GetCombatValues() const
{
	return CombatValues;
}

TArray<AArmy*> AArmy::GetNeighboringEnemyArmies() const
{
	TArray<AArmy*> NeighboringArmies;
	if (!GetCurrentTile()) return NeighboringArmies;
	for (const ATile* Neighbor : GetCurrentTile()->GetNeighbors())
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
	if (!GetCurrentTile()) return NeighboringDefenseBuildings;
	for (const ATile* Neighbor : GetCurrentTile()->GetNeighbors())
	{
		if (Neighbor &&
			Neighbor->GetClaimant() &&
			Neighbor->GetClaimant()->GetAffiliation() != GetAffiliation() &&
			Neighbor->GetBuilding() &&
			Neighbor->GetBuilding()->GetSettings()->bDefenseEnabled &&
			!Neighbor->GetBuilding()->GetIsUnderConstruction())
		{
			NeighboringDefenseBuildings.Add(Neighbor->GetBuilding());
		}
	}
	return NeighboringDefenseBuildings;
}

void AArmy::S_ArmyTakeDamage(int32 Damage)
{
	CombatValues->S_ReceiveDamage(Damage);
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

// -----------------Ravaging------------------------

bool AArmy::IsOnEnemyBuilding() const
{
	return GetCurrentTile() &&
		GetCurrentTile()->GetBuilding() &&
		GetCurrentTile()->GetClaimant() &&
		GetCurrentTile()->GetClaimant()->GetAffiliation() != GetAffiliation() &&
		!GetCurrentTile()->GetBuilding()->GetIsUnderConstruction();
}

bool AArmy::IsBuildingProtected() const
{
	return GetCurrentTile() &&
		GetCurrentTile()->GetBuilding() &&
		GetCurrentTile()->GetBuilding()->IsProtected();
}

float AArmy::GetRavageSpeed() const
{
	return RavageSpeed;
}

void AArmy::S_RavageEnemyBuilding()
{
	if (!GetCurrentTile() || !GetCurrentTile()->GetBuilding() || !IsOnEnemyBuilding() || IsBuildingProtected())
		return;
	GetCurrentTile()->S_Unbuild();
}

// -----------------Guarding------------------------

ATile* AArmy::GetGuardTile() const
{
	return GuardTile.Get();
}

void AArmy::S_SetGuardTile(ATile* NewGuardTile)
{
	GuardTile = NewGuardTile;
}

bool AArmy::IsOnGuardTile() const
{
	return GuardTile == GetCurrentTile();
}

TArray<ATile*> AArmy::FindPathToGuardTile() const
{
	if (!S_GetGameState()->GetTileMap()) return TArray<ATile*>();
	TArray<ATile*> Origin;
	Origin.Add(GetCurrentTile());
	const TArray<ATile*> ResultPath = S_GetGameState()->GetTileMap()->FindPathToNearestTile(
		Origin, EEntityType::Army,
		[this](const ATile* Tile)
		{
			return Tile && Tile == GuardTile;
		});
	return ResultPath;
}

TArray<ATile*> AArmy::FindPathToNearestEnemyToGuardTile() const
{
	if (!S_GetGameState()->GetTileMap()) return TArray<ATile*>();
	TArray<ATile*> SearchOrigin;
	SearchOrigin.Add(GuardTile.Get());
	TArray<ATile*> PathOrigin;
	PathOrigin.Add(GuardTile.Get());
	const TArray<ATile*> ResultPath = S_GetGameState()->GetTileMap()->FindPathToNearestTileFromSearchOrigin(
		SearchOrigin, PathOrigin, EEntityType::Army, [this](const ATile* Tile)
		{
			if (!Tile) return false;
			for (ATile* Neighbor : Tile->GetNeighbors())
			{
				if (Neighbor && Neighbor->GetArmy() && Neighbor->GetArmy()->GetAffiliation() != GetAffiliation())
					return true;
			}
			return false;
		});
	return ResultPath;
}

bool AArmy::HasEnemyInGuardTileRange()
{
	TArray<ATile*> Origin;
	Origin.Add(GuardTile.Get());
	ATile* EnemyOnTile = S_GetGameState()->GetTileMap()->FindNearestTileInRange(
		Origin, GuardModeInterceptingRange,
		EEntityType::None, [this](const ATile* Tile)
		{
			return Tile && Tile->GetArmy() && Tile->GetArmy()->GetAffiliation() != GetAffiliation();
		});
	return EnemyOnTile != nullptr;
}

// -----------------Intercepting------------------------

TWeakObjectPtr<AArmy> AArmy::GetInterceptArmy() const
{
	return InterceptArmy;
}

void AArmy::S_SetInterceptArmy(TWeakObjectPtr<AArmy> NewInterceptArmy)
{
	InterceptArmy = NewInterceptArmy;
}

TArray<ATile*> AArmy::FindPathToInterceptArmy() const
{
	if (!S_GetGameState()->GetTileMap()) return TArray<ATile*>();
	TArray<ATile*> Origin;
	Origin.Add(GetCurrentTile());
	const TArray<ATile*> ResultPath = S_GetGameState()->GetTileMap()->FindPathToNearestTile(
		Origin, EEntityType::Army,
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
	return ResultPath;
}
