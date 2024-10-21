// Copyright Epic Games, Inc. All Rights Reserved.

#include "StateTreeArmyConditions.h"

#include "StateTreeExecutionContext.h"
#include "GOTA/CoreSystems/Entity/Army.h"


bool FSTC_CurrentTileIsValidForRecruiting::TestCondition(FStateTreeExecutionContext& Context) const
{
	FInstanceDataType& InstanceData = Context.GetInstanceData(*this);
	if(!InstanceData.ArmyRef)
	{
		UE_LOG(LogTemp, Warning, TEXT("Army in context is null"))
		return false;
	}

	const bool bResult = InstanceData.ArmyRef.Get()->IsCurrentTileValidForRecruiting();
	return bResult ^ bInvert;
}
