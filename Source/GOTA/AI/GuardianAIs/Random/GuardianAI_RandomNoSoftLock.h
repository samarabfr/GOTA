// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GOTA/AI/GuardianAIs/GuardianAIController.h"
#include "GOTA/Settlement/ConstructionResources.h"
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
	float FoodBuildingFoodIncomeThreshold = 2.0f;

	UPROPERTY(EditDefaultsOnly)
	FConstructionResources BarracksIncomeThreshold = FConstructionResources(3, 2, 0.5);

	UPROPERTY(EditDefaultsOnly)
	int32 BarracksUnprotectedBuildingsThreshold = 5;
	
	void FigureOutBuilding();
	bool ShouldBuild() const;
	ATile* FindBuildableTile() const;
	TArray<ATile*> FindTilesWithMostNeighborPop() const;
	UBuildingSettings* SelectNewBuilding() const;
	
	// --------------------Logging----------------------
private:
	TMap<FName, int32> BuildingsCounter;

public:
	virtual TSharedPtr<FJsonObject> Log() override;
};
