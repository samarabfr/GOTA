// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "AIController.h"

#include "GuardianAI_Runner.generated.h"

class ATile;
class AGS_Ingame;

UCLASS(Blueprintable)
class GOTA_API AGuardianAI_Runner : public AAIController
{
	GENERATED_BODY()

	AGuardianAI_Runner();

	virtual void Tick(float DeltaSeconds) override;
	virtual void BeginPlay() override;

private:
	bool bWantsToMove = false;
	TWeakObjectPtr<AGS_Ingame> GameState;

	UPROPERTY(EditInstanceOnly)
	TWeakObjectPtr<ATile> TargetTile;

public:
	bool GetIsMoving();
	void SetIsMoving(bool NewIsMoving);
	ATile* GetTargetTile();
	void ResetToRandomTile();
	void ResetToRandomTileInRangeToTarget(int32 Range);
	void SteerPawn(float SteeringAngle);
};
