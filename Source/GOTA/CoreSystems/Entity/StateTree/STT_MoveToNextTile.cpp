// Fill out your copyright notice in the Description page of Project Settings.


#include "STT_MoveToNextTile.h"

#include "StateTreeExecutionContext.h"
#include "GOTA/CoreSystems/Entity/Army.h"

EStateTreeRunStatus FSTT_MoveToNextTile::EnterState(FStateTreeExecutionContext& Context,
                                                    const FStateTreeTransitionResult& Transition) const
{
	FInstanceDataType& InstanceData = Context.GetInstanceData(*this);
	if (!InstanceData.ArmyRef)
	{
		UE_LOG(LogTemp, Warning, TEXT("Army in context is null"))
		return EStateTreeRunStatus::Failed;
	}

	InstanceData.ArmyRef.Get()->MoveToNextTileOnPath();
	return EStateTreeRunStatus::Running;
}
