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

	// ------------------------------------ Production ------------------------------------

	UPROPERTY(EditDefaultsOnly, Category="Production")
	bool bProductionEnabled = false;
	
	UPROPERTY(EditDefaultsOnly, Category="Production", meta=(EditCondition = bProductionEnabled))
	float BaseProductionPerSecond = 0.0f;

	// ------------------------------------ Consumption ------------------------------------

	UPROPERTY(EditDefaultsOnly, Category="Consumption")
	bool bConsumptionEnabled = false;
	
	UPROPERTY(EditDefaultsOnly, Category="Consumption", meta=(EditCondition = bConsumptionEnabled))
	float BaseConsumptionPerSecond = 0.0f;

	UPROPERTY(EditDefaultsOnly, Category="Consumption", meta=(EditCondition = bConsumptionEnabled))
	FGameResources BaseResourceLimit = FGameResources::Zero();

	UPROPERTY(EditDefaultsOnly, Category="Consumption", meta=(EditCondition = bConsumptionEnabled))
	FGameResources ResourceLimitIncreasePerEfficiencyPercentage = FGameResources::Zero();

	// ------------------------------------ Civilian ------------------------------------

	UPROPERTY(EditDefaultsOnly, Category="Civilian")
	bool bCivilianEnabled = false;

	UPROPERTY(EditDefaultsOnly, Category="Civilian")
	TSubclassOf<ACivilian> CivilianClass;

	// In Seconds
	UPROPERTY(EditDefaultsOnly, Category="Civilian", meta=(Tooltip="In Seconds"))
	float CivilianGatheringTime = 0.0f;

	UPROPERTY(EditDefaultsOnly, Category="Civilian")
	float CivilianGatheringAmount = 0;

	UPROPERTY(EditDefaultsOnly, Category="Civilian")
	FGameResources CivilianInventoryLimit = FGameResources::Zero();

	// In Seconds
	UPROPERTY(EditDefaultsOnly, Category="Civilian", meta=(Tooltip="In Seconds"))
	float CivilianMoveTime = 0.0f;

	UPROPERTY(EditDefaultsOnly, Category="Civilian", meta=(Tooltip="In Seconds"))
	float CivilianRespawnTime = 0.0f;

	//--------------------------Army-------------------

	UPROPERTY(EditDefaultsOnly, Category="Army")
	bool bArmyEnabled = false;

	UPROPERTY(EditDefaultsOnly, Category="Army")
	TSubclassOf<AArmy> ArmyClass;

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

	//--------------------------Defense-------------------

	UPROPERTY(EditDefaultsOnly, Category="Defense")
	bool bDefenseEnabled = false;
	
	UPROPERTY(EditDefaultsOnly, Category="Defense")
	int32 RavageProtectionRange = 0;

	UPROPERTY(EditDefaultsOnly, Category="Defense")
	int32 DefenseIndividualMaxHP = 0;

	UPROPERTY(EditDefaultsOnly, Category="Defense")
	int32 DefenseIndividualAttack = 0;

	UPROPERTY(EditDefaultsOnly, Category="Defense")
	int32 DefenseIndividualCount = 0;

	UPROPERTY(EditDefaultsOnly, Category="Defense")
	float DefenseAttackTime = 0.0f;
};