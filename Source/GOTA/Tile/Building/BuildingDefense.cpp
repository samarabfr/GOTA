#include "BuildingDefense.h"

#include "BuildingSettings.h"
#include "Population.h"
#include "GOTA/Entity/Army.h"
#include "GOTA/Settlement/Settlement.h"
#include "GOTA/Utility/CombatValues.h"
#include "GOTA/GameplayFramework/GS_Ingame.h"
#include "GOTA/Tile/Tile.h"
#include "GOTA/Tilemap/TileMap.h"
#include "Net/UnrealNetwork.h"
#include "Net/Core/PushModel/PushModel.h"

// ------------------------------------ Replication Setup --------------------------------------

void UBuildingDefense::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	FDoRepLifetimeParams Params;
	Params.bIsPushBased = true;

	Params.Condition = COND_InitialOnly;
	Params.RepNotifyCondition = REPNOTIFY_Always;

	Params.Condition = COND_None;
	Params.RepNotifyCondition = REPNOTIFY_OnChanged;
	DOREPLIFETIME_WITH_PARAMS(UBuildingDefense, CombatValues, Params);
	DOREPLIFETIME_WITH_PARAMS(UBuildingDefense, AttackProgress, Params);
	DOREPLIFETIME_WITH_PARAMS(UBuildingDefense, bIsAttacking, Params);
}

bool UBuildingDefense::IsSupportedForNetworking() const
{
	return true;
}

UBuildingDefense::UBuildingDefense()
{
	CombatValues = CreateDefaultSubobject<UCombatValues>("Combat Values");
}

void UBuildingDefense::S_Init(UBuildingSettings* InSettings, ATile* InTile, ASettlement* InSettlement, AGS_Ingame* InGameState)
{
	Super::S_Init(InSettings, InTile, InSettlement, InGameState);
	CombatValues->SetIndividualAttack(GetSettings()->DefenseIndividualAttack);
	CombatValues->SetIndividualMaxHP(GetSettings()->DefenseIndividualMaxHP);
	CombatValues->SetIndividualCount(GetSettings()->DefenseIndividualCount);
	CombatValues->SetAttackSpeed(100 / GetSettings()->DefenseAttackTime);
	CombatValues->OnDeath.AddDynamic(this, &UBuildingDefense::S_HandleDeath);
	GetPopulation()->OnSizeChanged.AddDynamic(this, &UBuildingDefense::S_HandlePopSizeChanged);
}

// ---------------------------------------- Lifecycle ----------------------------------------

void UBuildingDefense::S_Tick(float DeltaSeconds)
{
	Super::S_Tick(DeltaSeconds);
	C_Tick(DeltaSeconds);
	if (!bIsAttacking && HasEnemyOnNeighboringTile())
	{
		bIsAttacking = true;
		AttackProgress = 0.0f;
	}
	else if (bIsAttacking && !HasEnemyOnNeighboringTile())
	{
		bIsAttacking = true;
		AttackProgress = 0.0f;
	}
	if (bIsAttacking && AttackProgress >= 100)
	{
		AttackProgress = 0.0f;
		S_AttackEnemy();
		MARK_PROPERTY_DIRTY_FROM_NAME(UBuildingDefense, AttackProgress, this)
	}
}

void UBuildingDefense::C_Tick(const float DeltaSeconds)
{
	Super::C_Tick(DeltaSeconds);
	if (bIsAttacking)
	{
		AttackProgress += CombatValues->GetAttackSpeed() * DeltaSeconds;
	}
}

void UBuildingDefense::BeginDestroy()
{
	Super::BeginDestroy();
}

TArray<AArmy*> UBuildingDefense::GetNeighboringEnemies() const
{
	TArray<AArmy*> NeighboringEnemies;
	if (!GetTile()) return NeighboringEnemies;
	for (const ATile* Neighbor : GetTile()->Neighbors)
	{
		if (Neighbor &&
			Neighbor->GetArmy() &&
			Neighbor->GetArmy()->GetAffiliation() != GetSettlement()->GetAffiliation())
		{
			NeighboringEnemies.Add(Neighbor->GetArmy());
		}
	}
	return NeighboringEnemies;
}

bool UBuildingDefense::HasEnemyOnNeighboringTile() const
{
	return GetNeighboringEnemies().Num() > 0;
}

void UBuildingDefense::S_StartAttacking()
{
	AttackProgress = 0.f;
	bIsAttacking = true;
}

void UBuildingDefense::S_BuildingDefenseTakeDamage(int32 Damage)
{
	CombatValues->SetCurrentTotalHP(CombatValues->GetCurrentTotalHP() - Damage);
}

void UBuildingDefense::S_FinishConstruction()
{
	Super::S_FinishConstruction();
	if (!S_GetGameState() || !S_GetGameState()->GetTileMap()) return;
	TArray<ATile*> Origin;
	Origin.Add(GetTile());
	TMap<ATile*, int8> Buildings = S_GetGameState()->GetTileMap()->FindAllTilesWithRangesInRange(Origin,
		GetSettings()->RavageProtectionRange, EEntityType::None,
		[](const ATile* BuildingTile)
		{
			return BuildingTile && BuildingTile->GetBuilding() && !BuildingTile->GetBuilding()->GetSettings()->
				bDefenseEnabled;
		});
	for (auto Building : Buildings)
	{
		if (Building.Key->GetClaimant()->GetAffiliation() == GetTile()->GetClaimant()->GetAffiliation())
		{
			Building.Key->GetBuilding()->S_RegisterProtector(this);
		}
	}
}

void UBuildingDefense::S_AttackEnemy()
{
	// choose enemy randomly
	TArray<AArmy*> AttackableEnemies = GetNeighboringEnemies();
	if (AttackableEnemies.Num() <= 0) return;
	const int32 RandomIndex = FMath::RandRange(0, AttackableEnemies.Num() - 1);
	AArmy* ChosenEnemy = AttackableEnemies[RandomIndex];
	// inflict damage
	ChosenEnemy->S_ArmyTakeDamage(CombatValues->GetAttack());
}

void UBuildingDefense::S_HandleDeath()
{
	if (!S_GetGameState() || !S_GetGameState()->GetTileMap()) return;
	TArray<ATile*> Origin;
	Origin.Add(GetTile());
	TMap<ATile*, int8> Buildings = S_GetGameState()->GetTileMap()->FindAllTilesWithRangesInRange(Origin,
		GetSettings()->RavageProtectionRange, EEntityType::None,
		[](const ATile* BuildingTile)
		{
			return BuildingTile && BuildingTile->GetBuilding() && !BuildingTile->GetBuilding()->GetSettings()->
				bDefenseEnabled;
		});
	for (auto Building : Buildings)
	{
		if (Building.Key->GetClaimant()->GetAffiliation() == GetTile()->GetClaimant()->GetAffiliation())
		{
			Building.Key->GetBuilding()->S_UnregisterProtector(this);
		}
	}
	GetTile()->S_Unbuild();
}

void UBuildingDefense::S_HandlePopSizeChanged(int16 ChangedBy)
{
	if (!CombatValues) return;
	CombatValues->SetIndividualCount(GetPopulation()->GetSize());
}
