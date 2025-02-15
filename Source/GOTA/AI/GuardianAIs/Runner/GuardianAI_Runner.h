// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "AIController.h"
#include "GOTA/ReinforcementLearning/Runner/RL_RunnerAgent.h"

#include "GuardianAI_Runner.generated.h"

class ULearningAgentsManager;
class ARL_RunnerManager;
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
	ULearningAgentsNeuralNetwork* NN_Encoder;
	UPROPERTY(EditDefaultsOnly)
	ULearningAgentsNeuralNetwork* NN_Policy;
	UPROPERTY(EditDefaultsOnly)
	ULearningAgentsNeuralNetwork* NN_Decoder;
	UPROPERTY(EditDefaultsOnly)
	ULearningAgentsNeuralNetwork* NN_Critic;
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<ARL_RunnerManager> ManagerClass;
	UPROPERTY(EditDefaultsOnly)
	FVector ResetOffset = FVector(0, 0, 50);
	
	bool bWantsToMove = false;

	UPROPERTY(EditInstanceOnly)
	TWeakObjectPtr<ATile> TargetTile;

public:
	virtual FTransform S_GetAgentTransform() const override;
	virtual void S_SetIsMoving(bool NewIsMoving) override;
	virtual ATile* S_GetTargetTile() const override;
	virtual void S_ResetToRandomTile() override;
	virtual void S_Steer(float SteeringAngle) override;
};
