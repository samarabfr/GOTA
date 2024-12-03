// Fill out your copyright notice in the Description page of Project Settings.


#include "StateTreeCivilianTasks.h"

#include "StateTreeExecutionContext.h"
#include "GOTA/CoreSystems/Entity/Civilian.h"


EStateTreeRunStatus FSTT_CivilianMoveToNextTile::EnterState(FStateTreeExecutionContext& Context,
	const FStateTreeTransitionResult& Transition) const
{
	FInstanceDataType& InstanceData = Context.GetInstanceData(*this);
	if (!InstanceData.CivilianRef)
	{
		UE_LOG(LogTemp, Warning, TEXT("Task failed: Civilian in context is null"))
		return EStateTreeRunStatus::Failed;
	}

	InstanceData.CivilianRef.Get()->S_StartMoveToNextTileOnPath();
	return EStateTreeRunStatus::Running;
}

EStateTreeRunStatus FSTT_Work::EnterState(FStateTreeExecutionContext& Context,
                                          const FStateTreeTransitionResult& Transition) const
{
	FInstanceDataType& InstanceData = Context.GetInstanceData(*this);
	if (!InstanceData.CivilianRef)
	{
		UE_LOG(LogTemp, Warning, TEXT("Task failed: Civilian in context is null"))
		return EStateTreeRunStatus::Failed;
	}

	InstanceData.CivilianRef.Get()->S_StartWorking();
	return EStateTreeRunStatus::Running;
}
