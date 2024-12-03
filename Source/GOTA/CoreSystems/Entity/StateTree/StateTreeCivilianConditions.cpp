// Copyright Epic Games, Inc. All Rights Reserved.

#include "StateTreeCivilianConditions.h"

#include "StateTreeExecutionContext.h"
#include "GOTA/CoreSystems/Entity/Civilian.h"

bool FSTC_IsPathValidCivilian::TestCondition(FStateTreeExecutionContext& Context) const
{
	FInstanceDataType& InstanceData = Context.GetInstanceData(*this);
	if (!InstanceData.CivilianRef)
	{
		UE_LOG(LogTemp, Warning, TEXT("Condition failed: Civilian in context is null"))
		return false;
	}

	const bool bResult = InstanceData.CivilianRef.Get()->IsPathValid();
	return bResult ^ bInvert;
}

bool FSTC_IsCurrentTileValidForWork::TestCondition(FStateTreeExecutionContext& Context) const
{
	FInstanceDataType& InstanceData = Context.GetInstanceData(*this);
	if (!InstanceData.CivilianRef)
	{
		UE_LOG(LogTemp, Warning, TEXT("Condition failed: Civilian in context is null"))
		return false;
	}

	const bool bResult = InstanceData.CivilianRef.Get()->IsCurrentTileValidForWork();
	return bResult ^ bInvert;
}
