#include "C:\UnrealProjects\GOTAFunzt\Intermediate\Build\Win64\x64\GOTAEditor\Development\UnrealEd\SharedPCH.UnrealEd.Project.NoValFmtStr.ValApi.Cpp20.InclOrderUnreal5_3.h"
#include "BuildingDirectProduction.h"

#include "BuildingSettings.h"
#include "GOTA/CoreSystems/Faction/Settlement/Settlement.h"
#include "Net/UnrealNetwork.h"
#include "Net/Core/PushModel/PushModel.h"

// ------------------------------------ Replication Setup --------------------------------------

void UBuildingDirectProduction::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	FDoRepLifetimeParams Params;
	Params.bIsPushBased = true;

	Params.Condition = COND_InitialOnly;
	Params.RepNotifyCondition = REPNOTIFY_Always;

	Params.Condition = COND_None;
	Params.RepNotifyCondition = REPNOTIFY_OnChanged;
	DOREPLIFETIME_WITH_PARAMS(UBuildingDirectProduction, DirectProductionProgress, Params);
}

bool UBuildingDirectProduction::IsSupportedForNetworking() const
{
	return true;
}

// ---------------------------------------- Lifecycle ----------------------------------------

void UBuildingDirectProduction::ServerTick(float DeltaSeconds)
{
	UBuilding::ServerTick(DeltaSeconds);
	if (DirectProductionProgress < Settings->DirectProductionTime)
	{
		DirectProductionProgress += DeltaSeconds;
	}
	else
	{
		S_ApplyDirectProduction();
		DirectProductionProgress = 0.0f;
		MARK_PROPERTY_DIRTY_FROM_NAME(UBuildingDirectProduction, DirectProductionProgress, this)
	}
}

void UBuildingDirectProduction::ClientTick(const float DeltaSeconds)
{
	Super::ClientTick(DeltaSeconds);
	DirectProductionProgress += DeltaSeconds;
}

void UBuildingDirectProduction::BeginDestroy()
{
	Super::BeginDestroy();
}

// --------------------------------------- Direct Production ---------------------------------------

void UBuildingDirectProduction::S_ApplyDirectProduction()
{
	FGameResources NewResources;
	switch (Settings->ProductionType)
	{
	case EProductionType::Food:
		NewResources.Food = Settings->DirectProductionAmount;
		break;

	case EProductionType::Wood:
		NewResources.Wood = Settings->DirectProductionAmount;
		break;

	case EProductionType::Stone:
		NewResources.Stone = Settings->DirectProductionAmount;
		break;
		
	default:
		break;
	}
	Settlement->S_AddResources(NewResources);
}
