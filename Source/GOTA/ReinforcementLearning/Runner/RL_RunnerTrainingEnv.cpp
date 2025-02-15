// Fill out your copyright notice in the Description page of Project Settings.


#include "RL_RunnerTrainingEnv.h"

#include "LearningAgentsRewards.h"
#include "RL_RunnerAgent.h"
#include "GOTA/CoreSystems/Tile/Tile.h"

URL_RunnerTrainingEnv::URL_RunnerTrainingEnv()
{
}

void URL_RunnerTrainingEnv::GatherAgentReward_Implementation(float& OutReward, const int32 AgentId)
{
	// specifies rewards
	IRL_RunnerAgent* Agent = Cast<IRL_RunnerAgent>(GetAgent(AgentId));
	if (!Agent) return;
	float Reward = 0;
	// progress reward
	Reward += LastDistance - GetDistanceToTarget(Agent);
	LastDistance = GetDistanceToTarget(Agent);
	// completion reward
	const FVector GuardianLocation = Agent->GetAgentTransform().GetLocation();
	const FVector TargetLocation = Agent->GetTargetTile()->GetActorTransform().GetLocation();
	Reward += ULearningAgentsRewards::MakeRewardOnLocationDifferenceBelowThreshold(
		GuardianLocation, TargetLocation, CompletionDistance, CompletionReward,
		L"LocationDifferenceBelowThreshold", true, this, AgentId,
		GuardianLocation, FLinearColor::Green);
	OutReward = Reward;
}

void URL_RunnerTrainingEnv::GatherAgentCompletion_Implementation(ELearningAgentsCompletion& OutCompletion,
                                                                 const int32 AgentId)
{
	IRL_RunnerAgent* Agent = Cast<IRL_RunnerAgent>(GetAgent(AgentId));
	if (!Agent) return;
	const FVector GuardianLocation = Agent->GetAgentTransform().GetLocation();
	const FVector TargetLocation = Agent->GetTargetTile()->GetActorTransform().GetLocation();
	const ELearningAgentsCompletion SuccessfulCompletion =
		ULearningAgentsCompletions::MakeCompletionOnLocationDifferenceBelowThreshold(
			GuardianLocation, TargetLocation, CompletionDistance);
	OutCompletion = SuccessfulCompletion;
}

void URL_RunnerTrainingEnv::ResetAgentEpisode_Implementation(const int32 AgentId)
{
	// return to starting conditions
	IRL_RunnerAgent* Agent = Cast<IRL_RunnerAgent>(GetAgent(AgentId));
	if (!Agent) return;
	Agent->ResetToRandomTile();
	LastDistance = GetDistanceToTarget(Agent);
}

float URL_RunnerTrainingEnv::GetDistanceToTarget(IRL_RunnerAgent* Agent)
{
	if (!Agent || !Agent->GetTargetTile()) return 0;
	const FVector GuardianLocation = Agent->GetAgentTransform().GetLocation();
	const FVector TargetLocation = Agent->GetTargetTile()->GetActorTransform().GetLocation();
	return FVector::Dist(GuardianLocation, TargetLocation);
}
