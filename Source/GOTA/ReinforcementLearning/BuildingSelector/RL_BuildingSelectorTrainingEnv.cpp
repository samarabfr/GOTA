// Fill out your copyright notice in the Description page of Project Settings.


#include "RL_BuildingSelectorTrainingEnv.h"

#include "LearningAgentsRewards.h"
#include "RL_BuildingSelectorAgent.h"
#include "GOTA/CoreSystems/Faction/Settlement/Settlement.h"
#include "GOTA/CoreSystems/GameplayFramework/GS_Ingame.h"

URL_BuildingSelectorTrainingEnv::URL_BuildingSelectorTrainingEnv()
{
}

void URL_BuildingSelectorTrainingEnv::Init(AGS_Ingame* InGameState, float InVictoryReward, float InLooseReward,
                                           TArray<FMilestone> InIncomeRewardMilestones,
                                           TArray<FMilestone> InResourcesRewardMilestones)
{
	GameState = InGameState;
	VictoryReward = InVictoryReward;
	LooseReward = InLooseReward;
	IncomeRewardMilestones = InIncomeRewardMilestones;
	ResourcesRewardMilestones = InResourcesRewardMilestones;
}

void URL_BuildingSelectorTrainingEnv::GatherAgentReward_Implementation(float& OutReward, const int32 AgentId)
{
	// specifies rewards
	IRL_BuildingSelectorAgent* Agent = Cast<IRL_BuildingSelectorAgent>(GetAgent(AgentId));
	if (!Agent) return;
	float Reward = 0;
	// completion reward
	if (GameState->GameEnded)
	{
		if (GameState->GetGameEnding() == EGameEnding::ColonistsWon
			&& Agent->GetAffiliation() == EAffiliation::Enemy ||
			GameState->GetGameEnding() == EGameEnding::NativesWon
			&& Agent->GetAffiliation() == EAffiliation::Ally)
		{
			Reward += VictoryReward;
			UE_LOG(LogTemp, Warning, TEXT("Rewarded %f for winning"), VictoryReward)
		}
		else
		{
			Reward += LooseReward;
			UE_LOG(LogTemp, Warning, TEXT("Rewarded %f for loosing"), LooseReward)
		}
	}
	// progress Reward von Milestones
	TArray<int32> MilestonesReached = Agent->GetMilestonesReached();
	if (MilestonesReached.Num() == 6)
	{
		if (ASettlement* Settlement = Agent->GetSettlement())
		{
			FGameResources Resources = Settlement->GetResources();
			FGameResources Income = Settlement->GetEffectivePredictedProduction();
			// Food
			if (MilestonesReached[0] < ResourcesRewardMilestones.Num() &&
				Resources.Food >= ResourcesRewardMilestones[MilestonesReached[0]].Goal)
			{
				Reward += ResourcesRewardMilestones[MilestonesReached[0]].Reward;
				Agent->IncrementMilestone(0);
				UE_LOG(LogTemp, Warning, TEXT("Rewarded %f for reaching a resource Milestone"),
				       ResourcesRewardMilestones[MilestonesReached[0]].Reward)
			}
			if (MilestonesReached[1] < IncomeRewardMilestones.Num() &&
				Income.Food >= IncomeRewardMilestones[MilestonesReached[1]].Goal)
			{
				Reward += IncomeRewardMilestones[MilestonesReached[1]].Reward;
				Agent->IncrementMilestone(1);
				UE_LOG(LogTemp, Warning, TEXT("Rewarded %f for reaching a income Milestone"),
				       IncomeRewardMilestones[MilestonesReached[1]].Reward)
			}
			// Wood
			if (MilestonesReached[2] < ResourcesRewardMilestones.Num() &&
				Resources.Food >= ResourcesRewardMilestones[MilestonesReached[2]].Goal)
			{
				Reward += ResourcesRewardMilestones[MilestonesReached[2]].Reward;
				Agent->IncrementMilestone(2);
				UE_LOG(LogTemp, Warning, TEXT("Rewarded %f for reaching a resource Milestone"),
				       ResourcesRewardMilestones[MilestonesReached[2]].Reward)
			}
			if (MilestonesReached[3] < IncomeRewardMilestones.Num() &&
				Income.Food >= IncomeRewardMilestones[MilestonesReached[3]].Goal)
			{
				Reward += IncomeRewardMilestones[MilestonesReached[3]].Reward;
				Agent->IncrementMilestone(3);
				UE_LOG(LogTemp, Warning, TEXT("Rewarded %f for reaching a income Milestone"),
				       IncomeRewardMilestones[MilestonesReached[3]].Reward)
			}
			// Stone
			if (MilestonesReached[4] < ResourcesRewardMilestones.Num() &&
				Resources.Food >= ResourcesRewardMilestones[MilestonesReached[4]].Goal)
			{
				Reward += ResourcesRewardMilestones[MilestonesReached[4]].Reward;
				Agent->IncrementMilestone(4);
				UE_LOG(LogTemp, Warning, TEXT("Rewarded %f for reaching a resource Milestone"),
				       ResourcesRewardMilestones[MilestonesReached[4]].Reward)
			}
			if (MilestonesReached[5] < IncomeRewardMilestones.Num() &&
				Income.Food >= IncomeRewardMilestones[MilestonesReached[5]].Goal)
			{
				Reward += IncomeRewardMilestones[MilestonesReached[5]].Reward;
				Agent->IncrementMilestone(5);
				UE_LOG(LogTemp, Warning, TEXT("Rewarded %f for reaching a income Milestone"),
				       IncomeRewardMilestones[MilestonesReached[5]].Reward)
			}
		}
	}
	// result
	OutReward = Reward;
}

void URL_BuildingSelectorTrainingEnv::GatherAgentCompletion_Implementation(ELearningAgentsCompletion& OutCompletion,
                                                                           const int32 AgentId)
{
}

void URL_BuildingSelectorTrainingEnv::ResetAgentEpisode_Implementation(const int32 AgentId)
{
	// return to starting conditions
}
