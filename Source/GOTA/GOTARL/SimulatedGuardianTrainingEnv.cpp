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
		float Reward = 0;
		// progress reward
		//Reward += LastDistance - GetDistanceToTarget(GuardianSimulator);
		//LastDistance = GetDistanceToTarget(GuardianSimulator);
		// completion reward
		const FVector GuardianLocation = GuardianSimulator->GetPawn()->GetActorTransform().GetLocation();
		const FVector TargetLocation = GuardianSimulator->GetTargetTile()->GetActorTransform().GetLocation();
		Reward += ULearningAgentsRewards::MakeRewardOnLocationDifferenceBelowThreshold(
			GuardianLocation, TargetLocation, CompletionDistance, CompletionReward,
			L"LocationDifferenceBelowThreshold", true, this, AgentId,
			GuardianLocation, FLinearColor::Green);
		OutReward = Reward;
	}
}

void USimulatedGuardianTrainingEnv::GatherAgentCompletion_Implementation(ELearningAgentsCompletion& OutCompletion,
                                                                         const int32 AgentId)
{
	if (AGuardianSimulator* GuardianSimulator = Cast<AGuardianSimulator>(GetAgent(AgentId)))
	{
		const FVector GuardianLocation = GuardianSimulator->GetPawn()->GetActorTransform().GetLocation();
		const FVector TargetLocation = GuardianSimulator->GetTargetTile()->GetActorTransform().GetLocation();
		const ELearningAgentsCompletion SuccessfulCompletion =
			ULearningAgentsCompletions::MakeCompletionOnLocationDifferenceBelowThreshold(
				GuardianLocation, TargetLocation, CompletionDistance);
		const ELearningAgentsCompletion FailedCompletion =
			ULearningAgentsCompletions::MakeCompletionOnLocationDifferenceAboveThreshold(
				GuardianLocation, TargetLocation, ResetDistance);
		OutCompletion = ULearningAgentsCompletions::CompletionOr(SuccessfulCompletion, FailedCompletion);
	}
}

void USimulatedGuardianTrainingEnv::ResetAgentEpisode_Implementation(const int32 AgentId)
{
	// return to starting conditions
	if (AGuardianSimulator* GuardianSimulator = Cast<AGuardianSimulator>(GetAgent(AgentId)))
	{
		GuardianSimulator->ResetToRandomTileInRangeToTarget(ResetTileRange);
		LastDistance = GetDistanceToTarget(GuardianSimulator);
	}
}

float USimulatedGuardianTrainingEnv::GetDistanceToTarget(AGuardianSimulator* GuardianSimulator)
{
	if (!GuardianSimulator || !GuardianSimulator->GetTargetTile()) return 0;
	const FVector GuardianLocation = GuardianSimulator->GetPawn()->GetActorTransform().GetLocation();
	const FVector TargetLocation = GuardianSimulator->GetTargetTile()->GetActorTransform().GetLocation();
	return FVector::Dist(GuardianLocation, TargetLocation);
}
