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

bool FSTC_IsCurrentTileBestWorkTile::TestCondition(FStateTreeExecutionContext& Context) const
{
	ACivilian* Civilian = Context.GetInstanceData(*this).CivilianRef.Get();
	if (!Civilian) return false;

	const bool bResult = Civilian->IsCurrentTileAmongBestWorkTiles();
	return bResult ^ bInvert;
}

bool FSTC_IsCurrentTilePriorityTile::TestCondition(FStateTreeExecutionContext& Context) const
{
	ACivilian* Civilian = Context.GetInstanceData(*this).CivilianRef.Get();
	if (!Civilian) return false;

	const bool bResult = Civilian->IsCurrentTilePriorityTile();
	return bResult ^ bInvert;
}

bool FSTC_HasPriorityTile::TestCondition(FStateTreeExecutionContext& Context) const
{
	ACivilian* Civilian = Context.GetInstanceData(*this).CivilianRef.Get();
	if (!Civilian) return false;

	const bool bResult = Civilian->GetPriorityTile() != nullptr;
	return bResult ^ bInvert;
}

bool FSTC_IsCurrentTileTheTarget::TestCondition(FStateTreeExecutionContext& Context) const
{
	ACivilian* Civilian = Context.GetInstanceData(*this).CivilianRef.Get();
	if (!Civilian) return false;

	const bool bResult = Civilian->GetPriorityTile() != nullptr && Civilian->IsCurrentTilePriorityTile() ||
		Civilian->GetPriorityTile() == nullptr && Civilian->IsCurrentTileAmongBestWorkTiles();
	return bResult ^ bInvert;
}
