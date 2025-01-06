// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "SimulatedGuardianManager.generated.h"

class ULearningAgentsManager;

UCLASS(Blueprintable)
class GOTA_API ASimulatedGuardianManager : public AActor
{
	GENERATED_BODY()

	// ----------------------- LifeCycle -----------------------
protected:
	ASimulatedGuardianManager();

	virtual void BeginPlay() override;
	
	// ----------------------- Learning Agents plugin -----------------------
private:
	UPROPERTY()
	ULearningAgentsManager* ManagerComponent;

	void Init();

public:
	void RegisterAgent(UObject* Agent);
};