// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "LearningAgentsTraining/Public/LearningAgentsTrainer.h"

#include "SimulatedGuardianTrainingEnv.generated.h"

UCLASS(Blueprintable)
class GOTA_API USimulatedGuardianTrainingEnv : public ULearningAgentsTrainer
{
	GENERATED_BODY()

	// ----------------------- LifeCycle -----------------------
protected:
	USimulatedGuardianTrainingEnv();

	// ----------------------- Learning Agents plugin -----------------------
public:
	virtual void GatherAgentReward_Implementation(float& OutReward, const int32 AgentId) override;

	virtual void
	GatherAgentCompletion_Implementation(ELearningAgentsCompletion& OutCompletion, const int32 AgentId) override;

	virtual void ResetAgentEpisode_Implementation(const int32 AgentId) override;
};
