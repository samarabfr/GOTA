// Copyright Epic Games, Inc. All Rights Reserved.

#include "StateTreeCivilianConditions.h"

#include "StateTreeExecutionContext.h"
#include "GOTA/Entity/Civilian.h"

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

bool FSTC_HasResourcesInInventory::TestCondition(FStateTreeExecutionContext& Context) const
{
	ACivilian* Civilian = Context.GetInstanceData(*this).CivilianRef.Get();
	if (!Civilian) return false;

	const bool bResult = Civilian->HasResourcesInInventory();
	return bResult ^ bInvert;
}

bool FSTC_IsInventoryFull::TestCondition(FStateTreeExecutionContext& Context) const
{
	ACivilian* Civilian = Context.GetInstanceData(*this).CivilianRef.Get();
	if (!Civilian) return false;

	const bool bResult = Civilian->IsInventoryFull();
	return bResult ^ bInvert;
}

bool FSTC_IsCurrentTileTheOriginBuilding::TestCondition(FStateTreeExecutionContext& Context) const
{
	ACivilian* Civilian = Context.GetInstanceData(*this).CivilianRef.Get();
	if (!Civilian) return false;

	const bool bResult = Civilian->IsCurrentTileOriginBuilding();
	return bResult ^ bInvert;
}
