// Fill out your copyright notice in the Description page of Project Settings.


#include "StateTreeCivilianTasks.h"

#include "StateTreeExecutionContext.h"
#include "GOTA/CoreSystems/Entity/Civilian.h"


EStateTreeRunStatus FSTT_CivilianMoveToNextTile::EnterState(FStateTreeExecutionContext& Context,
                                                            const FStateTreeTransitionResult& Transition) const
{
	ACivilian* Civilian = Context.GetInstanceData(*this).CivilianRef.Get();
	if (!Civilian) return EStateTreeRunStatus::Failed;

	Civilian->SetProgress(0.f);
	return EStateTreeRunStatus::Running;
}

EStateTreeRunStatus FSTT_CivilianMoveToNextTile::Tick(FStateTreeExecutionContext& Context, const float DeltaTime) const
{
	// TODO:: sehr ineffizient, progress muss aber für die UI irgendwie abgefragt werden können
	ACivilian* Civilian = Context.GetInstanceData(*this).CivilianRef.Get();
	if (!Civilian) return EStateTreeRunStatus::Failed;

	Civilian->AddProgress(Civilian->GetMovementRate() * DeltaTime);
	if (Civilian->GetProgress() >= 100.f)
	{
		Civilian->S_MoveToNextTileOnPath();
		Civilian->SetProgress(0.f);
	}
	return EStateTreeRunStatus::Running;
}

EStateTreeRunStatus FSTT_Work::EnterState(FStateTreeExecutionContext& Context,
                                          const FStateTreeTransitionResult& Transition) const
{
	ACivilian* Civilian = Context.GetInstanceData(*this).CivilianRef.Get();
	if (!Civilian) return EStateTreeRunStatus::Failed;

	Civilian->SetProgress(0.f);
	return EStateTreeRunStatus::Running;
}

EStateTreeRunStatus FSTT_Work::Tick(FStateTreeExecutionContext& Context, const float DeltaTime) const
{
	// TODO:: sehr ineffizient, progress muss aber für die UI irgendwie abgefragt werden können
	ACivilian* Civilian = Context.GetInstanceData(*this).CivilianRef.Get();
	if (!Civilian) return EStateTreeRunStatus::Failed;

	Civilian->AddProgress(Civilian->GetWorkRate() * DeltaTime);
	if (Civilian->GetProgress() >= 100.f)
	{
		Civilian->S_Work();
		Civilian->SetProgress(0.f);
	}
	return EStateTreeRunStatus::Running;
}

EStateTreeRunStatus FSTT_FindPathToNearestTileValidForWork::Tick(FStateTreeExecutionContext& Context,
                                                                 const float DeltaTime) const
{
	ACivilian* Civilian = Context.GetInstanceData(*this).CivilianRef.Get();
	if (!Civilian) return EStateTreeRunStatus::Failed;

	return Civilian->TryFindPathToNearestTileValidForWork()
		       ? EStateTreeRunStatus::Succeeded
		       : EStateTreeRunStatus::Running;
}

EStateTreeRunStatus FSTT_FindPathToPriorityTile::Tick(FStateTreeExecutionContext& Context, const float DeltaTime) const
{
	ACivilian* Civilian = Context.GetInstanceData(*this).CivilianRef.Get();
	if (!Civilian) return EStateTreeRunStatus::Failed;

	return Civilian->TryFindPathToPriorityTile()
			   ? EStateTreeRunStatus::Succeeded
			   : EStateTreeRunStatus::Running;
}
