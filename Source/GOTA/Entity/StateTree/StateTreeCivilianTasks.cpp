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

	return Civilian->S_TryFindPathToBestWorkTile()
		       ? EStateTreeRunStatus::Succeeded
		       : EStateTreeRunStatus::Running;
}

EStateTreeRunStatus FSTT_FindPathToPriorityTile::Tick(FStateTreeExecutionContext& Context, const float DeltaTime) const
{
	ACivilian* Civilian = Context.GetInstanceData(*this).CivilianRef.Get();
	if (!Civilian) return EStateTreeRunStatus::Failed;

	const bool bOverridePath = Context.GetInstanceData(*this).bOverridePathIn;
	if (!bOverridePath && !Civilian->IsPathEmpty()) return EStateTreeRunStatus::Succeeded;

	return Civilian->S_TryFindPathToPriorityTile()
		       ? EStateTreeRunStatus::Succeeded
		       : EStateTreeRunStatus::Running;
}

EStateTreeRunStatus FSTT_FindPathToOriginBuilding::Tick(FStateTreeExecutionContext& Context,
                                                        const float DeltaTime) const
{
	ACivilian* Civilian = Context.GetInstanceData(*this).CivilianRef.Get();
	if (!Civilian) return EStateTreeRunStatus::Failed;

	const bool bOverridePath = Context.GetInstanceData(*this).bOverridePathIn;
	if (!bOverridePath && !Civilian->IsPathEmpty()) return EStateTreeRunStatus::Succeeded;

	return Civilian->S_TryFindPathToOriginBuilding()
		       ? EStateTreeRunStatus::Succeeded
		       : EStateTreeRunStatus::Running;
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
