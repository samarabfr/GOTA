// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "GuardianAIController.generated.h"

UCLASS()
class GOTA_API AGuardianAIController : public AAIController
{
	GENERATED_BODY()

	// ----------------------- LifeCycle -----------------------
protected:
	AGuardianAIController();

public:
	virtual void S_Init(bool RunTraining = false);
	
	// ----------------------- Reinforcment Learning -----------------------
protected:
	bool bRunTraining = false;
	
	// ----------------------- Logging -----------------------
public:
	virtual void Log();
};
