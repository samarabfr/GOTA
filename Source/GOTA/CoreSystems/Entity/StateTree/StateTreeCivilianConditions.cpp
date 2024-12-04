// Copyright Epic Games, Inc. All Rights Reserved.

#include "StateTreeCivilianConditions.h"

#include "StateTreeExecutionContext.h"
#include "GOTA/CoreSystems/Entity/Civilian.h"

bool FSTC_IsPathValidCivilian::TestCondition(FStateTreeExecutionContext& Context) const
{
	ACivilian* Civilian = Context.GetInstanceData(*this).CivilianRef.Get();
	if (!Civilian) return false;

	const bool bResult = Civilian->IsPathValid();
	return bResult ^ bInvert;
}

bool FSTC_IsPathEmptyCivilian::TestCondition(FStateTreeExecutionContext& Context) const
{
	ACivilian* Civilian = Context.GetInstanceData(*this).CivilianRef.Get();
	if (!Civilian) return false;

	const bool bResult = Civilian->IsPathEmpty();
	return bResult ^ bInvert;
}

bool FSTC_IsCurrentTileValidForWork::TestCondition(FStateTreeExecutionContext& Context) const
{
	ACivilian* Civilian = Context.GetInstanceData(*this).CivilianRef.Get();
	if (!Civilian) return false;

	const bool bResult = Civilian->IsCurrentTileValidForWork();
	return bResult ^ bInvert;
}

bool FSTC_HasPiorityTileValidForWork::TestCondition(FStateTreeExecutionContext& Context) const
{
	ACivilian* Civilian = Context.GetInstanceData(*this).CivilianRef.Get();
	if (!Civilian) return false;

	const bool bResult = Civilian->IsPriorityTileValidForWork();
	return bResult ^ bInvert;
}
