#include "BuildingArmy.h"

#include "BuildingSettings.h"
#include "GOTA/CoreSystems/Entity/Army.h"
#include "GOTA/CoreSystems/Tile/Tile.h"
#include "Net/UnrealNetwork.h"
#include "Net/Core/PushModel/PushModel.h"

// ------------------------------------ Replication Setup --------------------------------------

void UBuildingArmy::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	FDoRepLifetimeParams Params;
	Params.bIsPushBased = true;

	Params.Condition = COND_InitialOnly;
	Params.RepNotifyCondition = REPNOTIFY_Always;

	Params.Condition = COND_None;
	Params.RepNotifyCondition = REPNOTIFY_OnChanged;
	DOREPLIFETIME_WITH_PARAMS(UBuildingArmy, Army, Params);
}

bool UBuildingArmy::IsSupportedForNetworking() const
{
	return true;
}

// ---------------------------------------- Lifecycle ----------------------------------------

void UBuildingArmy::S_Tick(float DeltaSeconds)
{
	UBuilding::S_Tick(DeltaSeconds);
	if (Army)
		Army->S_Tick(DeltaSeconds);
	// Army
	if (!GetIsUnderConstruction() && !Army)
	{
		if (ArmyRespawnTimer < Settings->ArmyRespawnTime)
		{
			ArmyRespawnTimer += DeltaSeconds;
		}
		else if (Tile->AcceptsArmy())
		{
			ArmyRespawnTimer = 0.0f;
			AArmy* NewArmy = Tile->GetWorld()->SpawnActor<AArmy>();
			NewArmy->S_Init(this, Tile);
			SetArmy(NewArmy);
		}
	}
}

void UBuildingArmy::C_Tick(const float DeltaSeconds)
{
	Super::C_Tick(DeltaSeconds);
}

void UBuildingArmy::BeginDestroy()
{
	Super::BeginDestroy();
}

// ---------------- Army ----------------

void UBuildingArmy::SetArmy(AArmy* NewArmy)
{
	Army = NewArmy;
	MARK_PROPERTY_DIRTY_FROM_NAME(UBuildingArmy, Army, this)
}
