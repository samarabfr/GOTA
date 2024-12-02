// Copyright Epic Games, Inc. All Rights Reserved.

#include "StateTreeArmyConditions.h"

#include "StateTreeExecutionContext.h"
#include "GOTA/CoreSystems/Entity/Army.h"


bool FSTC_CurrentTileIsValidForRecruiting::TestCondition(FStateTreeExecutionContext& Context) const
{
	FInstanceDataType& InstanceData = Context.GetInstanceData(*this);
	if (!InstanceData.ArmyRef)
	{
		UE_LOG(LogTemp, Warning, TEXT("Army in context is null"))
		return false;
	}

	const bool bResult = InstanceData.ArmyRef.Get()->IsCurrentTileValidForRecruiting();
	return bResult ^ bInvert;
}

bool FSTC_IsPathValid::TestCondition(FStateTreeExecutionContext& Context) const
{
	FInstanceDataType& InstanceData = Context.GetInstanceData(*this);
	if (!InstanceData.ArmyRef)
	{
		UE_LOG(LogTemp, Warning, TEXT("Army in context is null"))
		return false;
	}

	const bool bResult = InstanceData.ArmyRef.Get()->IsPathValid();
	return bResult ^ bInvert;
}

bool FSTC_HasEnemyInGarrisonModeRange::TestCondition(FStateTreeExecutionContext& Context) const
{
	FInstanceDataType& InstanceData = Context.GetInstanceData(*this);
	if (!InstanceData.ArmyRef)
	{
		UE_LOG(LogTemp, Warning, TEXT("Army in context is null"))
		return false;
	}

	const bool bResult = InstanceData.ArmyRef.Get()->HasEnemyInGarrisonModeRange();
	return bResult ^ bInvert;
}

bool FSTC_HasEnemyOnNeighboringTile::TestCondition(FStateTreeExecutionContext& Context) const
{
	FInstanceDataType& InstanceData = Context.GetInstanceData(*this);
	if (!InstanceData.ArmyRef)
	{
		UE_LOG(LogTemp, Warning, TEXT("Army in context is null"))
		return false;
	}

	const bool bResult = InstanceData.ArmyRef.Get()->HasEnemyOnNeighboringTile();
	return bResult ^ bInvert;
}

bool FSTC_IsOnEnemyBuilding::TestCondition(FStateTreeExecutionContext& Context) const
{
	FInstanceDataType& InstanceData = Context.GetInstanceData(*this);
	if (!InstanceData.ArmyRef)
	{
		UE_LOG(LogTemp, Warning, TEXT("Army in context is null"))
		return false;
	}

	const bool bResult = InstanceData.ArmyRef.Get()->IsOnEnemyBuilding();
	return bResult ^ bInvert;
}

bool FSTC_IsBuildingProtected::TestCondition(FStateTreeExecutionContext& Context) const
{
	FInstanceDataType& InstanceData = Context.GetInstanceData(*this);
	if (!InstanceData.ArmyRef)
	{
		UE_LOG(LogTemp, Warning, TEXT("Army in context is null"))
		return false;
	}

	const bool bResult = InstanceData.ArmyRef.Get()->IsBuildingProtected();
	return bResult ^ bInvert;
}

bool FSTC_IsOnGuardTile::TestCondition(FStateTreeExecutionContext& Context) const
{
	FInstanceDataType& InstanceData = Context.GetInstanceData(*this);
	if (!InstanceData.ArmyRef)
	{
		UE_LOG(LogTemp, Warning, TEXT("Army in context is null"))
		return false;
	}

	const bool bResult = InstanceData.ArmyRef.Get()->IsOnGuardTile();
	return bResult ^ bInvert;
}

bool FSTC_HasEnemyInGuardTileRange::TestCondition(FStateTreeExecutionContext& Context) const
{
	FInstanceDataType& InstanceData = Context.GetInstanceData(*this);
	if (!InstanceData.ArmyRef)
	{
		UE_LOG(LogTemp, Warning, TEXT("Army in context is null"))
		return false;
	}

	const bool bResult = InstanceData.ArmyRef.Get()->HasEnemyInGuardTileRange();
	return bResult ^ bInvert;
}

bool FSTC_HasGuardTile::TestCondition(FStateTreeExecutionContext& Context) const
{
	FInstanceDataType& InstanceData = Context.GetInstanceData(*this);
	if (!InstanceData.ArmyRef)
	{
		UE_LOG(LogTemp, Warning, TEXT("Army in context is null"))
		return false;
	}

	const bool bResult = InstanceData.ArmyRef.Get()->GetGuardTile() == nullptr;
	return bResult ^ bInvert;
}

bool FSTC_HasInterceptArmy::TestCondition(FStateTreeExecutionContext& Context) const
{
	FInstanceDataType& InstanceData = Context.GetInstanceData(*this);
	if (!InstanceData.ArmyRef)
	{
		UE_LOG(LogTemp, Warning, TEXT("Army in context is null"))
		return false;
	}

	const bool bResult = InstanceData.ArmyRef.Get()->GetInterceptArmy().IsValid();
	return bResult ^ bInvert;
}
