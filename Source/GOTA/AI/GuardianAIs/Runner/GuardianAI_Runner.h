// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "AIController.h"
#include "GOTA/ReinforcementLearning/Runner/RL_RunnerAgent.h"

#include "GuardianAI_Runner.generated.h"

class AGS_Ingame;
class ULearningAgentsNeuralNetwork;
class ATile;

UCLASS(Blueprintable)
class GOTA_API AGuardianAI_Runner : public AAIController, public IRL_RunnerAgent
{
	GENERATED_BODY()

	// ----------------------- LifeCycle -----------------------

private:
	virtual void Tick(float DeltaSeconds) override;
	virtual void BeginPlay() override;

protected:
	AGuardianAI_Runner();

	
	// ----------------------- Utility -----------------------
private:
	TWeakObjectPtr<AGS_Ingame> GameState;

	// ----------------------- Movement control -----------------------
private:
	UPROPERTY(EditDefaultsOnly)
	TWeakObjectPtr<ULearningAgentsNeuralNetwork> NN_Encoder;
	UPROPERTY(EditDefaultsOnly)
	TWeakObjectPtr<ULearningAgentsNeuralNetwork> NN_Policy;
	UPROPERTY(EditDefaultsOnly)
	TWeakObjectPtr<ULearningAgentsNeuralNetwork> NN_Decoder;
	UPROPERTY(EditDefaultsOnly)
	TWeakObjectPtr<ULearningAgentsNeuralNetwork> NN_Critic;

	bool bWantsToMove = false;

	UPROPERTY(EditInstanceOnly)
	TWeakObjectPtr<ATile> TargetTile;

public:
	virtual FTransform GetAgentTransform() const override;
	virtual void SetIsMoving(bool NewIsMoving) override;
	virtual ATile* GetTargetTile() const override;
	virtual void ResetToRandomTile() override;
	virtual void Steer(float SteeringAngle) override;
};
