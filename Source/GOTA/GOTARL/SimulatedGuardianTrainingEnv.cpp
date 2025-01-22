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
		float Reward = LastDistance - GetDistanceToTarget(GuardianSimulator);
		LastDistance = GetDistanceToTarget(GuardianSimulator);
		OutReward = Reward;
	}
}

void USimulatedGuardianTrainingEnv::GatherAgentCompletion_Implementation(ELearningAgentsCompletion& OutCompletion,
                                                                         const int32 AgentId)
{
	if (AGuardianSimulator* GuardianSimulator = Cast<AGuardianSimulator>(GetAgent(AgentId)))
	{
		// early termination conditions
		FVector GuardianLocation = GuardianSimulator->GetPawn()->GetActorTransform().GetLocation();
		FVector TargetLocation = GuardianSimulator->GetTargetTile()->GetActorTransform().GetLocation();
		OutCompletion = ULearningAgentsCompletions::MakeCompletionOnLocationDifferenceBelowThreshold(
			GuardianLocation, TargetLocation, 500.0f);
	}
}

void USimulatedGuardianTrainingEnv::ResetAgentEpisode_Implementation(const int32 AgentId)
{
	// return to starting conditions
	if (AGuardianSimulator* GuardianSimulator = Cast<AGuardianSimulator>(GetAgent(AgentId)))
	{
		GuardianSimulator->ResetToRandomTile();
		LastDistance = GetDistanceToTarget(GuardianSimulator);
	}
}

float USimulatedGuardianTrainingEnv::GetDistanceToTarget(AGuardianSimulator* GuardianSimulator)
{
	if (!GuardianSimulator || !GuardianSimulator->GetTargetTile()) return 0;
	FVector GuardianLocation = GuardianSimulator->GetPawn()->GetActorTransform().GetLocation();
	FVector TargetLocation = GuardianSimulator->GetTargetTile()->GetActorTransform().GetLocation();
	return FVector::Dist(GuardianLocation, TargetLocation);
}
