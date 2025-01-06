// Fill out your copyright notice in the Description page of Project Settings.


#include "SimulatedGuardianTrainingEnv.h"

#include "LearningAgentsRewards.h"
#include "SimulatedGuardian.h"

USimulatedGuardianTrainingEnv::USimulatedGuardianTrainingEnv()
{
}

void USimulatedGuardianTrainingEnv::GatherAgentReward_Implementation(float& OutReward, const int32 AgentId)
{
	// specifies rewards
	if (ASimulatedGuardian* Guardian = Cast<ASimulatedGuardian>(GetAgent(AgentId)))
	{
		ULearningAgentsRewards::MakeRewardFromLocationDifference(
			Guardian->GetTargetTile()->GetActorTransform().GetLocation(),
			Guardian->GetActorTransform().GetLocation(), 10000.0f);
	}
}

void USimulatedGuardianTrainingEnv::GatherAgentCompletion_Implementation(ELearningAgentsCompletion& OutCompletion,
                                                                         const int32 AgentId)
{
	if (ASimulatedGuardian* Guardian = Cast<ASimulatedGuardian>(GetAgent(AgentId)))
	{
		// early termination conditions
		OutCompletion = ULearningAgentsCompletions::MakeCompletionOnLocationDifferenceBelowThreshold(
			Guardian->GetTargetTile()->GetActorTransform().GetLocation(),
			Guardian->GetActorTransform().GetLocation(), 100.0f, ELearningAgentsCompletion::Truncation);
	}
}

void USimulatedGuardianTrainingEnv::ResetAgentEpisode_Implementation(const int32 AgentId)
{
	// TODO: return to starting conditions
}
