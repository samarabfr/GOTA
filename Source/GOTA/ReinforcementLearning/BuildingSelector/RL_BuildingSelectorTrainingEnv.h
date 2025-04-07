// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "LearningAgentsTrainingEnvironment.h"

#include "RL_BuildingSelectorTrainingEnv.generated.h"

class AGS_Ingame;
class IRL_BuildingSelectorAgent;
class URL_BuildingSelector;

USTRUCT()
struct FMilestone
{
	GENERATED_BODY()

	float Goal;
	float Reward;
};

UCLASS(Blueprintable)
class GOTA_API URL_BuildingSelectorTrainingEnv : public ULearningAgentsTrainingEnvironment
{
	GENERATED_BODY()

	// ----------------------- LifeCycle -----------------------
private:
	TWeakObjectPtr<AGS_Ingame> GameState;

protected:
	URL_BuildingSelectorTrainingEnv();

public:
	void Init(AGS_Ingame* InGameState, float InVictoryReward, float InLooseReward,
	          TArray<FMilestone> InIncomeRewardMilestones,
	          TArray<FMilestone> InResourcesRewardMilestones);

	// ----------------------- Learning -----------------------
private:
	float VictoryReward = 1000;
	float LooseReward = -1000;
	float RewardPerPop = 1;
	int16 LastPopSize = 0;
	TArray<FMilestone> IncomeRewardMilestones;
	TArray<FMilestone> ResourcesRewardMilestones;

public:
	virtual void GatherAgentReward_Implementation(float& OutReward, const int32 AgentId) override;

	virtual void
	GatherAgentCompletion_Implementation(ELearningAgentsCompletion& OutCompletion, const int32 AgentId) override;

	virtual void ResetAgentEpisode_Implementation(const int32 AgentId) override;
};
