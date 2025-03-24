// Fill out your copyright notice in the Description page of Project Settings.


#include "StateTreeCivilianTasks.h"

#include "StateTreeExecutionContext.h"
#include "GOTA/Entity/Civilian.h"


EStateTreeRunStatus FSTT_CivilianMoveToNextTile::EnterState(FStateTreeExecutionContext& Context,
                                                            const FStateTreeTransitionResult& Transition) const
{
	ACivilian* Civilian = Context.GetInstanceData(*this).CivilianRef.Get();
	if (!Civilian) return EStateTreeRunStatus::Failed;

	TWeakObjectPtr<ACivilian> WeakCivilian = Civilian;
	Civilian->S_StartProgresser([WeakCivilian]()
	                            {
		                            return WeakCivilian.IsValid() ? WeakCivilian->GetMovementRate() : 0.f;
	                            },
	                            [WeakCivilian]()
	                            {
		                            if (WeakCivilian.IsValid()) WeakCivilian->S_MoveToNextTileOnPath();
	                            });
	return EStateTreeRunStatus::Running;
}

void FSTT_CivilianMoveToNextTile::ExitState(FStateTreeExecutionContext& Context,
                                            const FStateTreeTransitionResult& Transition) const
{
	ACivilian* Civilian = Context.GetInstanceData(*this).CivilianRef.Get();
	if (!Civilian) return;

	Civilian->S_StopProgresser();
}

EStateTreeRunStatus FSTT_Work::EnterState(FStateTreeExecutionContext& Context,
                                          const FStateTreeTransitionResult& Transition) const
{
	ACivilian* Civilian = Context.GetInstanceData(*this).CivilianRef.Get();
	if (!Civilian) return EStateTreeRunStatus::Failed;

	TWeakObjectPtr<ACivilian> WeakCivilian = Civilian;
	Civilian->S_StartProgresser([WeakCivilian]()
	                            {
		                            return WeakCivilian.IsValid() ? WeakCivilian->GetWorkRate() : 0.f;
	                            },
	                            [WeakCivilian]() { if (WeakCivilian.IsValid()) WeakCivilian->S_Work(); });
	return EStateTreeRunStatus::Running;
}

void FSTT_Work::ExitState(FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition) const
{
	ACivilian* Civilian = Context.GetInstanceData(*this).CivilianRef.Get();
	if (!Civilian) return;

	Civilian->S_StopProgresser();
}

EStateTreeRunStatus FSTT_FindPathToBestWorkTile::Tick(FStateTreeExecutionContext& Context,
                                                      const float DeltaTime) const
{
	ACivilian* Civilian = Context.GetInstanceData(*this).CivilianRef.Get();
	if (!Civilian) return EStateTreeRunStatus::Failed;
	const bool bOverridePath = Context.GetInstanceData(*this).bOverridePathIn;
	if (!bOverridePath && !Civilian->IsPathEmpty()) return EStateTreeRunStatus::Succeeded;
	// do pathfinding asynchronously with tasks
	UE::Tasks::TTask<TArray<ATile*>> PathfindingTask = Context.GetInstanceData(*this).PathfindingTask;
	// launch task if it isnt yet or has completed unsuccesfully
	if (!PathfindingTask.IsValid() || PathfindingTask.IsCompleted() && PathfindingTask.GetResult().IsEmpty())
	{
		Context.GetInstanceData(*this).PathfindingTask = UE::Tasks::Launch(
			UE_SOURCE_LOCATION,
			[Civilian]
			{
				return Civilian->FindPathToBestWorkTile();
			}
		);
		return EStateTreeRunStatus::Running;
	}
	// complete when task has found path
	if (PathfindingTask.IsCompleted() && !PathfindingTask.GetResult().IsEmpty())
	{
		Civilian->S_SetPath(PathfindingTask.GetResult());
		return EStateTreeRunStatus::Succeeded;
	}
	// task is still computing
	return EStateTreeRunStatus::Running;
}

void FSTT_FindPathToBestWorkTile::ExitState(FStateTreeExecutionContext& Context,
                                            const FStateTreeTransitionResult& Transition) const
{
	Context.GetInstanceData(*this).PathfindingTask = {};
}

EStateTreeRunStatus FSTT_FindPathToPriorityTile::Tick(FStateTreeExecutionContext& Context, const float DeltaTime) const
{
	ACivilian* Civilian = Context.GetInstanceData(*this).CivilianRef.Get();
	if (!Civilian) return EStateTreeRunStatus::Failed;
	const bool bOverridePath = Context.GetInstanceData(*this).bOverridePathIn;
	if (!bOverridePath && !Civilian->IsPathEmpty()) return EStateTreeRunStatus::Succeeded;
	// do pathfinding asynchronously with tasks
	UE::Tasks::TTask<TArray<ATile*>> PathfindingTask = Context.GetInstanceData(*this).PathfindingTask;
	// launch task if it isnt yet or has completed unsuccesfully
	if (!PathfindingTask.IsValid() || PathfindingTask.IsCompleted() && PathfindingTask.GetResult().IsEmpty())
	{
		Context.GetInstanceData(*this).PathfindingTask = UE::Tasks::Launch(
			UE_SOURCE_LOCATION,
			[Civilian]
			{
				return Civilian->FindPathToPriorityTile();
			}
		);
		return EStateTreeRunStatus::Running;
	}
	// complete when task has found path
	if (PathfindingTask.IsCompleted() && !PathfindingTask.GetResult().IsEmpty())
	{
		Civilian->S_SetPath(PathfindingTask.GetResult());
		return EStateTreeRunStatus::Succeeded;
	}
	// task is still computing
	return EStateTreeRunStatus::Running;
}

void FSTT_FindPathToPriorityTile::ExitState(FStateTreeExecutionContext& Context,
	const FStateTreeTransitionResult& Transition) const
{
	Context.GetInstanceData(*this).PathfindingTask = {};
}

EStateTreeRunStatus FSTT_FindPathToOriginBuilding::Tick(FStateTreeExecutionContext& Context,
                                                        const float DeltaTime) const
{
	ACivilian* Civilian = Context.GetInstanceData(*this).CivilianRef.Get();
	if (!Civilian) return EStateTreeRunStatus::Failed;
	const bool bOverridePath = Context.GetInstanceData(*this).bOverridePathIn;
	if (!bOverridePath && !Civilian->IsPathEmpty()) return EStateTreeRunStatus::Succeeded;
	// do pathfinding asynchronously with tasks
	UE::Tasks::TTask<TArray<ATile*>> PathfindingTask = Context.GetInstanceData(*this).PathfindingTask;
	// launch task if it isnt yet or has completed unsuccesfully
	if (!PathfindingTask.IsValid() || PathfindingTask.IsCompleted() && PathfindingTask.GetResult().IsEmpty())
	{
		Context.GetInstanceData(*this).PathfindingTask = UE::Tasks::Launch(
			UE_SOURCE_LOCATION,
			[Civilian]
			{
				return Civilian->FindPathToOriginBuilding();
			}
		);
		return EStateTreeRunStatus::Running;
	}
	// complete when task has found path
	if (PathfindingTask.IsCompleted() && !PathfindingTask.GetResult().IsEmpty())
	{
		Civilian->S_SetPath(PathfindingTask.GetResult());
		return EStateTreeRunStatus::Succeeded;
	}
	// task is still computing
	return EStateTreeRunStatus::Running;
}

void FSTT_FindPathToOriginBuilding::ExitState(FStateTreeExecutionContext& Context,
	const FStateTreeTransitionResult& Transition) const
{
	Context.GetInstanceData(*this).PathfindingTask = {};
}

EStateTreeRunStatus FSTT_UnloadResources::EnterState(FStateTreeExecutionContext& Context,
                                                     const FStateTreeTransitionResult& Transition) const
{
	ACivilian* Civilian = Context.GetInstanceData(*this).CivilianRef.Get();
	if (!Civilian) return EStateTreeRunStatus::Failed;

	TWeakObjectPtr<ACivilian> WeakCivilian = Civilian;
	Civilian->S_UnloadResources();
	return EStateTreeRunStatus::Succeeded;
}
