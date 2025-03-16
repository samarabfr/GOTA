// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "GameFramework/Actor.h"
#include "GuardianAI_RandomNoSoftLock.generated.h"

class ATile;
class AGuardian;
class AGS_Ingame;
class ASettlement;
class UBuildingSettings;

UCLASS()
class GOTA_API AGuardianAI_RandomNoSoftLock : public AAIController
{
	GENERATED_BODY()
	
	// ----------------------- LifeCycle -----------------------

private:
	virtual void Tick(float DeltaSeconds) override;

protected:
	AGuardianAI_RandomNoSoftLock();

public:
	void S_Tick(const float DeltaSeconds);
	void C_Tick(const float DeltaSeconds);

	// ----------------------- Utility -----------------------
private:
	UPROPERTY()
	AGS_Ingame* GameState;
	UPROPERTY()
	AGuardian* PossessedGuardian;
	UPROPERTY()
	ASettlement* Settlement;
	virtual void OnPossess(APawn* InPawn) override;

	// ----------------------- Building Selection -----------------------
	
	void FigureOutBuilding();
	bool ShouldBuild() const;
	ATile* FindBuildableTile() const;
	UBuildingSettings* SelectNewBuilding() const;

	// --------------------Army----------------------
private:
	UPROPERTY(VisibleInstanceOnly)
	float SendArmiesIntervalTimeLeft = 0.0f;

	UPROPERTY(EditDefaultsOnly)
	float SendArmiesIntervalTime = 90.0f;
};
