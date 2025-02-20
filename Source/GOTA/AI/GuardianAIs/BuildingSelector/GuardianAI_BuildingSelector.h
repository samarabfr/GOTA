// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "AIController.h"
#include "GOTA/ReinforcementLearning/BuildingSelector/RL_BuildingSelectorAgent.h"

#include "GuardianAI_BuildingSelector.generated.h"

class ASettlement;
class AGuardian;
class ULearningAgentsManager;
class ARL_BuildingSelectorManager;
class AGS_Ingame;
class ULearningAgentsNeuralNetwork;
class ATile;

UCLASS(Blueprintable)
class GOTA_API AGuardianAI_BuildingSelector : public AAIController, public IRL_BuildingSelectorAgent
{
	GENERATED_BODY()

	// ----------------------- LifeCycle -----------------------

private:
	virtual void Tick(float DeltaSeconds) override;
	virtual void BeginPlay() override;

protected:
	AGuardianAI_BuildingSelector();


	// ----------------------- Utility -----------------------
private:
	TWeakObjectPtr<AGS_Ingame> GameState;
	TWeakObjectPtr<AGuardian> PossessedGuardian;
	TWeakObjectPtr<ASettlement> Settlement;
	virtual void OnPossess(APawn* InPawn) override;

	// ----------------------- Reinforcement Learning -----------------------
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
	TSubclassOf<ARL_BuildingSelectorManager> ManagerClass;

	void RandomlyPlaceBuilding(UBuildingSettings* Building);

public:
	virtual ASettlement* GetSettlement() override;

	virtual TArray<UBuildingSettings*> GetAvailableBuildings() override;
	virtual void SelectBuilding(UBuildingSettings* Building) override;
};
