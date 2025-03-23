// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GOTA/AI/GuardianAIs/GuardianAIController.h"
#include "GuardianAI_RandomNoSoftLock.generated.h"

class ATile;
class AGuardian;
class AGS_Ingame;
class ASettlement;
class UBuildingSettings;

UCLASS()
class GOTA_API AGuardianAI_RandomNoSoftLock : public AGuardianAIController
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
private:
	UPROPERTY(EditDefaultsOnly)
	float FoodThreshold = 300.0f;
	
	void FigureOutBuilding();
	bool ShouldBuild() const;
	ATile* FindBuildableTile() const;
	UBuildingSettings* SelectNewBuilding() const;
	
	// --------------------Logging----------------------
private:
	TMap<FName, int32> BuildingsCounter;
	void LogBuildings();

public:
	virtual void Log() override;
};
