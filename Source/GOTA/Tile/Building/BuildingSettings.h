#pragma once

#include "GameplayTagContainer.h"
#include "GOTA/Settlement/ConstructionResources.h"
#include "GOTA/Tile/TileGraphics/GameplayTagRule.h"
#include "GOTA/Utility/Enums.h"
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
	FConstructionResources Cost;

	UPROPERTY(EditDefaultsOnly, Category="Building")
	int32 Housing = 0;

	// ------------------------------------ Production ------------------------------------

	UPROPERTY(EditDefaultsOnly, Category="Production")
	bool bProductionEnabled = false;

	UPROPERTY(EditDefaultsOnly, Category="Production", meta=(EditCondition = bProductionEnabled))
	EProductionType ProductionType = EProductionType::None;

	UPROPERTY(EditDefaultsOnly, Category="Production", meta=(EditCondition = bProductionEnabled))
	float BaseProductionPerSecond = 0.0f;

	// ------------------------------------ Consumption ------------------------------------

	UPROPERTY(EditDefaultsOnly, Category="Consumption")
	bool bConsumptionEnabled = false;

	UPROPERTY(EditDefaultsOnly, Category="Consumption", meta=(EditCondition = bConsumptionEnabled))
	EResource ConsumptionType = EResource::None;

	UPROPERTY(EditDefaultsOnly, Category="Consumption", meta=(EditCondition = bConsumptionEnabled))
	float BaseConsumptionPerSecond = 0.0f;

	UPROPERTY(EditDefaultsOnly, Category="Consumption", meta=(EditCondition = bConsumptionEnabled))
	float BaseResourceLimit = 0.0f;

	UPROPERTY(EditDefaultsOnly, Category="Consumption", meta=(EditCondition = bConsumptionEnabled))
	float ResourceLimitIncreasePerEfficiencyPercentage = 0.0f;

	// ------------------------------------ Civilian ------------------------------------

	UPROPERTY(EditDefaultsOnly, Category="SpecialType|Civilian",
		meta=(EditCondition = "!bArmyEnabled && !bDefenseEnabled"))
	bool bCivilianEnabled = false;

	UPROPERTY(EditDefaultsOnly, Category="SpecialType|Civilian", meta=(EditCondition = bCivilianEnabled))
	TSubclassOf<ACivilian> CivilianClass;

	// In Seconds
	UPROPERTY(EditDefaultsOnly, Category="SpecialType|Civilian", meta=(Tooltip="In Seconds"),
		meta=(EditCondition = bCivilianEnabled))
	float CivilianGatheringTime = 0.0f;

	UPROPERTY(EditDefaultsOnly, Category="SpecialType|Civilian", meta=(EditCondition = bCivilianEnabled))
	float CivilianGatheringAmount = 0.0f;

	UPROPERTY(EditDefaultsOnly, Category="SpecialType|Civilian", meta=(EditCondition = bCivilianEnabled))
	float CivilianStorageLimit = 0.0f;

	UPROPERTY(EditDefaultsOnly, Category="SpecialType|Civilian", meta=(EditCondition = bCivilianEnabled))
	float CivilianStorageLimitIncreasePerEfficiencyPercentage = 0.0f;

	// In Seconds
	UPROPERTY(EditDefaultsOnly, Category="SpecialType|Civilian", meta=(Tooltip="In Seconds"),
		meta=(EditCondition = bCivilianEnabled))
	float CivilianMoveTime = 0.0f;

	UPROPERTY(EditDefaultsOnly, Category="SpecialType|Civilian", meta=(Tooltip="In Seconds"),
		meta=(EditCondition = bCivilianEnabled))
	float CivilianRespawnTime = 0.0f;

	//--------------------------Army-------------------

	UPROPERTY(EditDefaultsOnly, Category="SpecialType|Army",
		meta=(EditCondition = "!bCivilianEnabled && !bDefenseEnabled"))
	bool bArmyEnabled = false;

	UPROPERTY(EditDefaultsOnly, Category="SpecialType|Army", meta=(EditCondition = bArmyEnabled))
	TSubclassOf<AArmy> ArmyClass;

	UPROPERTY(EditDefaultsOnly, Category="SpecialType|Army", meta=(EditCondition = bArmyEnabled))
	float SecondsPerRecruitCycle = 0.0f;

	UPROPERTY(EditDefaultsOnly, Category="SpecialType|Army", meta=(EditCondition = bArmyEnabled))
	float ArmyMoveTime = 0.0f;

	UPROPERTY(EditDefaultsOnly, Category="SpecialType|Army", meta=(EditCondition = bArmyEnabled))
	float ArmyRespawnTime = 0.0f;

	UPROPERTY(EditDefaultsOnly, Category="SpecialType|Army", meta=(EditCondition = bArmyEnabled))
	int32 ArmyIndividualMaxHP = 0;

	UPROPERTY(EditDefaultsOnly, Category="SpecialType|Army", meta=(EditCondition = bArmyEnabled))
	int32 ArmyIndividualAttack = 0;

	UPROPERTY(EditDefaultsOnly, Category="SpecialType|Army", meta=(EditCondition = bArmyEnabled))
	int32 ArmyIndividualCount = 0;

	UPROPERTY(EditDefaultsOnly, Category="SpecialType|Army", meta=(EditCondition = bArmyEnabled))
	float ArmyAttackTime = 0.0f;

	UPROPERTY(EditDefaultsOnly, Category="SpecialType|Army", meta=(EditCondition = bArmyEnabled))
	float ArmyRavageTime = 0.0f;

	//--------------------------Defense-------------------

	UPROPERTY(EditDefaultsOnly, Category="SpecialType|Defense",
		meta=(EditCondition = "!bCivilianEnabled && !bArmyEnabled"))
	bool bDefenseEnabled = false;

	UPROPERTY(EditDefaultsOnly, Category="SpecialType|Defense", meta=(EditCondition = bDefenseEnabled))
	int32 RavageProtectionRange = 0;

	UPROPERTY(EditDefaultsOnly, Category="SpecialType|Defense", meta=(EditCondition = bDefenseEnabled))
	int32 DefenseIndividualMaxHP = 0;

	UPROPERTY(EditDefaultsOnly, Category="SpecialType|Defense", meta=(EditCondition = bDefenseEnabled))
	int32 DefenseIndividualAttack = 0;

	UPROPERTY(EditDefaultsOnly, Category="SpecialType|Defense", meta=(EditCondition = bDefenseEnabled))
	int32 DefenseIndividualCount = 0;

	UPROPERTY(EditDefaultsOnly, Category="SpecialType|Defense", meta=(EditCondition = bDefenseEnabled))
	float DefenseAttackTime = 0.0f;
};
