// Fill out your copyright notice in the Description page of Project Settings.


#include "SimulatedGuardianTrainingEnv.h"

#include "LearningAgentsRewards.h"
#include "GuardianSimulator.h"

USimulatedGuardianTrainingEnv::USimulatedGuardianTrainingEnv()
{
}

void USimulatedGuardianTrainingEnv::GatherAgentReward_Implementation(float& OutReward, const int32 AgentId)
{
	// specifies rewards
	if (AGuardianSimulator* GuardianSimulator = Cast<AGuardianSimulator>(GetAgent(AgentId)))
	{
		OutReward = ULearningAgentsRewards::MakeRewardFromLocationDifference(
			GuardianSimulator->GetTargetTile()->GetActorTransform().GetLocation(),
			GuardianSimulator->GetPawn()->GetActorTransform().GetLocation(), 10000.0f);
	}
}

void USimulatedGuardianTrainingEnv::GatherAgentCompletion_Implementation(ELearningAgentsCompletion& OutCompletion,
                                                                         const int32 AgentId)
{
	if (AGuardianSimulator* GuardianSimulator = Cast<AGuardianSimulator>(GetAgent(AgentId)))
	{
		// early termination conditions
		OutCompletion = ULearningAgentsCompletions::MakeCompletionOnLocationDifferenceBelowThreshold(
			GuardianSimulator->GetTargetTile()->GetActorTransform().GetLocation(),
			GuardianSimulator->GetPawn()->GetActorTransform().GetLocation(), 100.0f, ELearningAgentsCompletion::Truncation);
	}
}

void USimulatedGuardianTrainingEnv::ResetAgentEpisode_Implementation(const int32 AgentId)
{
	// return to starting conditions
	if (AGuardianSimulator* GuardianSimulator = Cast<AGuardianSimulator>(GetAgent(AgentId)))
	{
		GuardianSimulator->ResetToRandomTile();
	}
}
