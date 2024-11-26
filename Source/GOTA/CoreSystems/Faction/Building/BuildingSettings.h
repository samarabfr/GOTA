#pragma once

#include "GameplayTagContainer.h"
#include "GOTA/CoreSystems/Faction/Settlement/GameResources.h"
#include "GOTA/CoreSystems/Tile/GameplayTagRule.h"
#include "GOTA/CoreSystems/Utility/Enums.h"
#include "BuildingSettings.generated.h"

class AArmy;
class ACivilian;

UCLASS()
class GOTA_API UBuildingSettings : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, Category="Building")
	FName Name = "Unnamed";

	UPROPERTY(EditDefaultsOnly, Category="Building")
	FText Description;

	UPROPERTY(EditDefaultsOnly, Category="Building")
	FGameplayTagContainer GameplayTags;

	UPROPERTY(EditDefaultsOnly, Category="Building")
	TArray<FGameplayTagRule> PlacementRules;

	UPROPERTY(EditDefaultsOnly, Category="Building")
	FGameResources Cost;

	UPROPERTY(EditDefaultsOnly, Category="Building")
	int32 Housing = 0;

	UPROPERTY(EditDefaultsOnly, Category="Building")
	EProductionType ProductionType = EProductionType::None;

	UPROPERTY(EditDefaultsOnly, Category="Building")
	EConsumptionType ConsumptionType = EConsumptionType::None;

	// ------------------------------------ Direct Production ------------------------------------

	UPROPERTY(EditDefaultsOnly, Category="Income")
	bool bDirectProductionEnabled = false;

	// In Seconds
	UPROPERTY(EditDefaultsOnly, Category="Income", meta=(Tooltip="In Seconds"))
	float DirectProductionTime = 0.0f;

	UPROPERTY(EditDefaultsOnly, Category="Income")
	float DirectProductionAmount = 0.0f;

	UPROPERTY(EditDefaultsOnly, Category="Income")
	float DirectConsumptionAmount = 0.0f;

	// ------------------------------------ Civilian Entity ------------------------------------

	UPROPERTY(EditDefaultsOnly, Category="Civilian")
	bool bCivilianEnabled = false;

	UPROPERTY(EditDefaultsOnly, Category="Civilian")
	TSubclassOf<ACivilian> CivilianClass;

	// In Seconds
	UPROPERTY(EditDefaultsOnly, Category="Civilian", meta=(Tooltip="In Seconds"))
	float CivilianProductionTime = 0.0f;

	UPROPERTY(EditDefaultsOnly, Category="Civilian")
	float CivilianProductionAmount = 0;

	UPROPERTY(EditDefaultsOnly, Category="Civilian")
	float CivilianConsumptionAmount = 0;

	// In Seconds
	UPROPERTY(EditDefaultsOnly, Category="Civilian", meta=(Tooltip="In Seconds"))
	float CivilianMoveTime = 0.0f;

	// Multiplies the theoretical optimum production to estimate the time spent not working
	UPROPERTY(EditDefaultsOnly, Category="Civilian"
		, meta=(Tooltip="Multiplies the theoretical optimum production to abstract the time spent not working"))
	float CivilianPredictedIncomeFactor = 0.7f;

	// ------------------------------------ Utility ------------------------------------
public:
	// returns the predicted Production per Second
	float GetDefaultPredictedProduction() const;

	// returns the predicted Consumption per Second
	float GetDefaultPredictedConsumption() const;

	//--------------------------Army-------------------

	UPROPERTY(EditDefaultsOnly, Category="Army")
	bool bArmyEnabled = false;

	UPROPERTY(EditDefaultsOnly, Category="Army")
	float SecondsPerRecruitCycle = 0.0f;

	UPROPERTY(EditDefaultsOnly, Category="Army")
	float ArmyMoveTime = 0.0f;

	UPROPERTY(EditDefaultsOnly, Category="Army")
	float ArmyRespawnTime = 0.0f;

	UPROPERTY(EditDefaultsOnly, Category="Army")
	int32 ArmyIndividualMaxHP = 0;

	UPROPERTY(EditDefaultsOnly, Category="Army")
	int32 ArmyIndividualAttack = 0;

	UPROPERTY(EditDefaultsOnly, Category="Army")
	int32 ArmyIndividualCount = 0;

	UPROPERTY(EditDefaultsOnly, Category="Army")
	float ArmyAttackTime = 0.0f;

	UPROPERTY(EditDefaultsOnly, Category="Army")
	float ArmyRavageTime = 0.0f;

};