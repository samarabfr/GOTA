// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "RL_RunnerAgent.generated.h"

class ATile;
// This class does not need to be modified.
UINTERFACE()
class URL_RunnerAgent : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class GOTA_API IRL_RunnerAgent
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	virtual FTransform GetAgentTransform() const;
	virtual ATile* GetTargetTile() const;

	virtual void ResetToRandomTile();
	virtual void SetIsMoving(bool InIsMoving);
	virtual void Steer(float SteeringAngle);
};
