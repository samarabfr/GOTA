// Fill out your copyright notice in the Description page of Project Settings.


#include "STT_FindPathToNearestEnemy.h"

#include "StateTreeExecutionContext.h"
#include "GOTA/CoreSystems/Entity/Army.h"

EStateTreeRunStatus FSTT_FindPathToNearestEnemy::EnterState(FStateTreeExecutionContext& Context,
                                                     const FStateTreeTransitionResult& Transition) const
{
	FInstanceDataType& InstanceData = Context.GetInstanceData(*this);
	if (!InstanceData.ArmyRef)
	{
		UE_LOG(LogTemp, Warning, TEXT("Army in context is null"))
		return EStateTreeRunStatus::Failed;
	}

	const bool bHasPath = InstanceData.ArmyRef.Get()->TryFindPathToNearestEnemy();
	return bHasPath ? EStateTreeRunStatus::Succeeded : EStateTreeRunStatus::Failed;
}
