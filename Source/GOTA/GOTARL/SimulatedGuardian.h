// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "GOTA/CoreSystems/Guardian/Guardian.h"

#include "SimulatedGuardian.generated.h"

UCLASS(Blueprintable)
class GOTA_API ASimulatedGuardian : public AGuardian
{
	GENERATED_BODY()

	virtual void Tick(float DeltaSeconds) override;
	virtual void BeginPlay() override;

private:
	FVector2d MoveDirection = FVector2d::ZeroVector;
	bool bWantsToMove = false;
	TWeakObjectPtr<ATile> TargetTile;
	TWeakObjectPtr<AGS_Ingame> GameState;

public:
	void SetMoveDirection(const FVector2d& NewDirection) {MoveDirection = NewDirection;};
	FVector2d GetMoveDirection() { return MoveDirection; };
	bool GetIsMoving() { return bWantsToMove; };
	void SetIsMoving(bool NewIsMoving) { bWantsToMove = NewIsMoving; };
	ATile* GetTargetTile() { return TargetTile.Get(); }
	void ResetToRandomTile();
};
