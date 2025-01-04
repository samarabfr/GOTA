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
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	virtual void Tick(float DeltaSeconds) override;

public:
	void S_Init();

	void S_Tick(const float DeltaSeconds);
	void C_Tick(const float DeltaSeconds);

private:
	virtual void BeginDestroy() override;

	// ----------------------- Learning Agents plugin -----------------------
private:
	UPROPERTY()
	ULearningAgentsManager* ManagerComponent;

public:
	void RegisterAgent(UObject* Agent);
};