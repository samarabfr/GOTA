// Copyright Epic Games, Inc. All Rights Reserved.

#include "StateTreeArmyConditions.h"

#include "StateTreeExecutionContext.h"
#include "GOTA/CoreSystems/Entity/Army.h"


bool FSTC_CurrentTileIsValidForRecruiting::TestCondition(FStateTreeExecutionContext& Context) const
{
	AArmy* Army = Context.GetInstanceData(*this).ArmyRef.Get();
	if (!Army) return false;

	const bool bResult = Army->IsCurrentTileValidForRecruiting();
	return bResult ^ bInvert;
}

bool FSTC_IsPathValidArmy::TestCondition(FStateTreeExecutionContext& Context) const
{
	AArmy* Army = Context.GetInstanceData(*this).ArmyRef.Get();
	if (!Army) return false;

	const bool bResult = Army->IsPathValid();
	return bResult ^ bInvert;
}

bool FSTC_IsPathEmptyArmy::TestCondition(FStateTreeExecutionContext& Context) const
{
	AArmy* Army = Context.GetInstanceData(*this).ArmyRef.Get();
	if (!Army) return false;

	const bool bResult = Army->IsPathEmpty();
	return bResult ^ bInvert;
}

bool FSTC_HasEnemyInGarrisonModeRange::TestCondition(FStateTreeExecutionContext& Context) const
{
	AArmy* Army = Context.GetInstanceData(*this).ArmyRef.Get();
	if (!Army) return false;

	const bool bResult = Army->HasEnemyInGarrisonModeRange();
	return bResult ^ bInvert;
}

bool FSTC_HasEnemyOnNeighboringTile::TestCondition(FStateTreeExecutionContext& Context) const
{
	AArmy* Army = Context.GetInstanceData(*this).ArmyRef.Get();
	if (!Army) return false;

	const bool bResult = Army->HasEnemyOnNeighboringTile();
	return bResult ^ bInvert;
}

bool FSTC_IsOnEnemyBuilding::TestCondition(FStateTreeExecutionContext& Context) const
{
	AArmy* Army = Context.GetInstanceData(*this).ArmyRef.Get();
	if (!Army) return false;

	const bool bResult = Army->IsOnEnemyBuilding();
	return bResult ^ bInvert;
}

bool FSTC_IsBuildingProtected::TestCondition(FStateTreeExecutionContext& Context) const
{
	AArmy* Army = Context.GetInstanceData(*this).ArmyRef.Get();
	if (!Army) return false;

	const bool bResult = Army->IsBuildingProtected();
	return bResult ^ bInvert;
}

bool FSTC_IsOnGuardTile::TestCondition(FStateTreeExecutionContext& Context) const
{
	AArmy* Army = Context.GetInstanceData(*this).ArmyRef.Get();
	if (!Army) return false;

	const bool bResult = Army->IsOnGuardTile();
	return bResult ^ bInvert;
}

bool FSTC_HasEnemyInGuardTileRange::TestCondition(FStateTreeExecutionContext& Context) const
{
	AArmy* Army = Context.GetInstanceData(*this).ArmyRef.Get();
	if (!Army) return false;

	const bool bResult = Army->HasEnemyInGuardTileRange();
	return bResult ^ bInvert;
}

bool FSTC_HasGuardTile::TestCondition(FStateTreeExecutionContext& Context) const
{
	AArmy* Army = Context.GetInstanceData(*this).ArmyRef.Get();
	if (!Army) return false;

	const bool bResult = Army->GetGuardTile() == nullptr;
	return bResult ^ bInvert;
}

bool FSTC_HasInterceptArmy::TestCondition(FStateTreeExecutionContext& Context) const
{
	AArmy* Army = Context.GetInstanceData(*this).ArmyRef.Get();
	if (!Army) return false;

	const bool bResult = Army->GetInterceptArmy().IsValid();
	return bResult ^ bInvert;
}
