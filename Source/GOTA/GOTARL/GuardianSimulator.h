// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "AIController.h"
#include "GOTA/CoreSystems/Guardian/Guardian.h"

#include "GuardianSimulator.generated.h"

UCLASS(Blueprintable)
class GOTA_API AGuardianSimulator : public AAIController
{
	GENERATED_BODY()

	AGuardianSimulator();

	virtual void Tick(float DeltaSeconds) override;
	virtual void BeginPlay() override;

private:
	FVector MoveDirection = FVector::ZeroVector;
	bool bWantsToMove = false;
	TWeakObjectPtr<ATile> TargetTile;
	TWeakObjectPtr<AGS_Ingame> GameState;

public:
	void SetMoveDirection(FVector NewDirection);
	FVector GetMoveDirection();
	bool GetIsMoving();
	void SetIsMoving(bool NewIsMoving);
	ATile* GetTargetTile();
	void ResetToRandomTile();
};
