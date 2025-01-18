#include "Civilian.h"
#include "CivilianSettings.h"
#include "StateTree.h"
#include "GOTA/CoreSystems/Faction/Building/Building.h"
#include "GOTA/CoreSystems/Faction/Building/BuildingSettings.h"
#include "GOTA/CoreSystems/Faction/Settlement/Settlement.h"
#include "GOTA/CoreSystems/Tile/Tile.h"
#include "GOTA/CoreSystems/GameplayFramework/GS_Ingame.h"
#include "GOTA/CoreSystems/Tile/TileMap.h"
#include "Net/UnrealNetwork.h"
#include "Net/Core/PushModel/PushModel.h"
#include "StateTree/StateTreeCivilianComponent.h"

void ACivilian::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	FDoRepLifetimeParams Params;
	Params.bIsPushBased = true;

	Params.Condition = COND_InitialOnly;
	Params.RepNotifyCondition = REPNOTIFY_Always;
	DOREPLIFETIME_WITH_PARAMS(ACivilian, OriginBuilding, Params);
	DOREPLIFETIME_WITH_PARAMS(ACivilian, Settings, Params);
	DOREPLIFETIME_WITH_PARAMS(ACivilian, WorkAmount, Params);
	DOREPLIFETIME_WITH_PARAMS(ACivilian, MovementRate, Params);
	DOREPLIFETIME_WITH_PARAMS(ACivilian, ResourceInventoryLimit, Params);

	Params.Condition = COND_None;
	Params.RepNotifyCondition = REPNOTIFY_OnChanged;
	DOREPLIFETIME_WITH_PARAMS(ACivilian, Progress, Params);
	DOREPLIFETIME_WITH_PARAMS(ACivilian, CurrentTile, Params);
	DOREPLIFETIME_WITH_PARAMS(ACivilian, NetLocation, Params);
	DOREPLIFETIME_WITH_PARAMS(ACivilian, bProgresserActive, Params);
	DOREPLIFETIME_WITH_PARAMS(ACivilian, ProgressRate, Params);
	DOREPLIFETIME_WITH_PARAMS(ACivilian, PriorityTile, Params);
	DOREPLIFETIME_WITH_PARAMS(ACivilian, ResourceInventory, Params);
}

// ----------------------- LifeCycle -----------------------

ACivilian::ACivilian()
{
	static ConstructorHelpers::FObjectFinder<UCivilianSettings> SettingsFinder(
		TEXT("/Game/CoreSystems/Entity/DA_Civilian"));
	Settings = SettingsFinder.Object;

	bReplicates = true;
	bAlwaysRelevant = true;
	bReplicateUsingRegisteredSubObjectList = true;
	SetNetUpdateFrequency(0.1f);

	RootComponent = CreateDefaultSubobject<USceneComponent>("ROOT");
	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>("Static Mesh");
	MeshComponent->SetupAttachment(RootComponent);
	MeshComponent->SetRelativeScale3D(FVector(1, 1, 1));
	// I still don't understand why i need to set both: the ResponseChannel and CollisionEnabled
	// but this way it will only collide with ray casts, as intended
	MeshComponent->SetSimulatePhysics(false);
	MeshComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	MeshComponent->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	MeshComponent->SetCollisionResponseToChannel(ECC_Visibility, ECollisionResponse::ECR_Block);

	StateTree = CreateDefaultSubobject<UStateTreeCivilianComponent>("StateTree");
	StateTree->SetStartLogicAutomatically(false);
}

void ACivilian::S_Init(UBuilding* InBuilding, ATile* SpawnTile)
{
	GameState = GetWorld()->GetGameState<AGS_Ingame>();
	OriginBuilding = InBuilding;
	CurrentTile = SpawnTile;
	FVector NewLocation = FVector();
	SpawnTile->AddCivilian(this, NewLocation);
	SetNetLocation(NewLocation);

	const UBuildingSettings* BuildingSettings = OriginBuilding->GetSettings();
	WorkAmount = BuildingSettings->CivilianProductionAmount;
	MovementRate = 100 / BuildingSettings->CivilianMoveTime;
	ResourceInventoryLimit = BuildingSettings->CivilianInventoryLimit;

	const FSoftObjectPath StateTreePath(TEXT("/Game/CoreSystems/Entity/ST_Civilian"));
	UStateTree* LoadedStateTree = Cast<UStateTree>(StateTreePath.TryLoad());
	if (LoadedStateTree) StateTree->SetStateTree(LoadedStateTree);
	StateTree->StartLogic();
}

void ACivilian::S_Tick(const float DeltaSeconds)
{
	if (bProgresserActive)
	{
		const float NewProgressRate = CalculateProgressRate();
		if (ProgressRate != NewProgressRate)
		{
			ProgressRate = NewProgressRate;
			MARK_PROPERTY_DIRTY_FROM_NAME(ACivilian, ProgressRate, this)
			ForceNetUpdate();
		}
		ProgressTick(DeltaSeconds);
		if (Progress >= 100.f)
		{
			FinishProgress();
			Progress = 0.f;
			MARK_PROPERTY_DIRTY_FROM_NAME(ACivilian, Progress, this)
			ForceNetUpdate();
		}
	}
}

void ACivilian::C_Tick(const float DeltaSeconds)
{
	if (bProgresserActive)
	{
		ProgressTick(DeltaSeconds);
	}
}

void ACivilian::BeginDestroy()
{
	Super::BeginDestroy();
	S_HandleDeath();
}

void ACivilian::S_HandleDeath()
{
	if (!CurrentTile.IsValid()) return;
	CurrentTile->RemoveCivilian(this);
	Destroy();
}

void ACivilian::S_StartProgresser(const std::function<float()>& ProgressRateCalculator,
                                  const std::function<void()>& Finisher)
{
	if (!ProgressRateCalculator || !Finisher) return;
	bProgresserActive = true;
	Progress = 0.f;
	MARK_PROPERTY_DIRTY_FROM_NAME(ACivilian, bProgresserActive, this)
	MARK_PROPERTY_DIRTY_FROM_NAME(ACivilian, Progress, this)
	CalculateProgressRate = ProgressRateCalculator;
	FinishProgress = Finisher;
	ProgressRate = CalculateProgressRate();
	MARK_PROPERTY_DIRTY_FROM_NAME(ACivilian, ProgressRate, this)
	ForceNetUpdate();
}

void ACivilian::S_StopProgresser()
{
	bProgresserActive = false;
	Progress = 0.f;
	MARK_PROPERTY_DIRTY_FROM_NAME(ACivilian, bProgresserActive, this)
	MARK_PROPERTY_DIRTY_FROM_NAME(ACivilian, Progress, this)
	CalculateProgressRate = nullptr;
	FinishProgress = nullptr;
	ProgressRate = 0.f;
	MARK_PROPERTY_DIRTY_FROM_NAME(ACivilian, ProgressRate, this)
	ForceNetUpdate();
}

void ACivilian::ProgressTick(float DeltaSeconds)
{
	if (Progress == 100.f) return;
	Progress += ProgressRate * DeltaSeconds;
	if (Progress > 100.f)
		Progress = 100.f;
}

// ----------------- Working ------------------------

bool ACivilian::IsTileValidForWork(const ATile* Tile) const
{
	return false;
}

void ACivilian::S_Work()
{
}

bool ACivilian::S_TryFindPathToBestWorkTile()
{
	return S_TryFindPathToClosestWorkTile();
}

bool ACivilian::IsCurrentTileAmongBestWorkTiles()
{
	return IsTileValidForWork(GetCurrentTile());
}

bool ACivilian::IsCurrentTilePriorityTile() const
{
	return GetCurrentTile() == GetPriorityTile();
}

float ACivilian::GetWorkRate() const
{
	return 100.f / OriginBuilding->GetSettings()->CivilianProductionTime * OriginBuilding->GetEfficiency();
}

bool ACivilian::HasResourcesInInventory() const
{
	return GetResourceInventory().Food > 0 || GetResourceInventory().Stone > 0 || GetResourceInventory().Wood > 0;
}

bool ACivilian::IsInventoryFull() const
{
	return GetResourceInventory() == GetResourceInventoryLimit();
}

bool ACivilian::IsCurrentTileOriginBuilding() const
{
	if (!GetCurrentTile() || !GetOriginBuilding() || !GetOriginBuilding()->GetTile()) return false;
	return GetCurrentTile() == GetOriginBuilding()->GetTile();
}

void ACivilian::S_UnloadResources()
{
	GetOriginBuilding()->GetSettlement()->S_AddResources(GetResourceInventory());
	ResourceInventory = FGameResources::Zero();
}

void ACivilian::S_AddResources(const FGameResources Resources)
{
	if (Resources <= FGameResources::Zero() || IsInventoryFull()) return;

	if (ResourceInventory.Food < ResourceInventoryLimit.Food)
	{
		ResourceInventory.Food = FMath::Min(ResourceInventoryLimit.Food, ResourceInventory.Food + Resources.Food);
	}
	if (ResourceInventory.Stone < ResourceInventoryLimit.Stone)
	{
		ResourceInventory.Stone = FMath::Min(ResourceInventoryLimit.Stone, ResourceInventory.Stone + Resources.Stone);
	}
	if (ResourceInventory.Wood < ResourceInventoryLimit.Wood)
	{
		ResourceInventory.Wood = FMath::Min(ResourceInventoryLimit.Wood, ResourceInventory.Wood + Resources.Wood);
	}
}

bool ACivilian::S_TryFindPathToClosestWorkTile()
{
	if (!CurrentTile.IsValid()) return false;
	if (IsTileValidForWork(GetCurrentTile())) return true;
	Path = GameState->GetTileMap()->FindPathToNearestTile(GetCurrentTile(), EEntityType::Civilian,
	                                                      [this](const ATile* Tile)
	                                                      {
		                                                      return IsTileValidForWork(Tile);
	                                                      });
	return !Path.IsEmpty();
}

bool ACivilian::S_TryFindPathToWorkTileClosestToSettlement()
{
	if (!GetOriginBuilding() ||
		!GetOriginBuilding()->GetSettlement() ||
		GetOriginBuilding()->GetSettlement()->ClaimedTiles.IsEmpty())
		return false;
	for (ATile* ClaimedTile : GetOriginBuilding()->GetSettlement()->ClaimedTiles)
	{
		if (IsTileValidForWork(ClaimedTile))
		{
			Path = GameState->GetTileMap()->GetPath(CurrentTile.Get(), ClaimedTile);
			if (!Path.IsEmpty()) return true;
		}
	}
	Path = GameState->GetTileMap()->FindPathToNearestTile(GetOriginBuilding()->GetSettlement()->ClaimedTiles,
	                                                      CurrentTile.Get(), EEntityType::Civilian,
	                                                      [this](const ATile* Tile)
	                                                      {
		                                                      return IsTileValidForWork(Tile);
	                                                      });
	return !Path.IsEmpty();
}

bool ACivilian::S_TryFindPathToPriorityTile()
{
	if (!CurrentTile.IsValid() || !PriorityTile.IsValid()) return false;
	Path = GameState->GetTileMap()->GetPath(GetCurrentTile(), GetPriorityTile(), EEntityType::Civilian);
	return !Path.IsEmpty();
}

bool ACivilian::S_TryFindPathToOriginBuilding()
{
	if (!CurrentTile.IsValid() || !OriginBuilding.IsValid() || !GetOriginBuilding()->GetTile()) return false;
	Path = GameState->GetTileMap()->GetPath(GetCurrentTile(), GetOriginBuilding()->GetTile(), EEntityType::Civilian);
	return !Path.IsEmpty();
}

// ----------------- Moving ------------------------

void ACivilian::OnRep_NetLocation()
{
	SetActorLocation(NetLocation);
}

void ACivilian::SetNetLocation(const FVector& NewNetLocation)
{
	SetActorLocation(NewNetLocation);
	NetLocation = NewNetLocation;
	MARK_PROPERTY_DIRTY_FROM_NAME(ACivilian, NetLocation, this)
}

void ACivilian::S_MoveToNextTileOnPath()
{
	ATile* NewCurrent = nullptr;
	if (!Path.IsEmpty())
	{
		NewCurrent = Path.Pop();
	}
	if (!NewCurrent) return;
	CurrentTile->RemoveCivilian(this);
	FVector NewLocation = FVector();
	NewCurrent->AddCivilian(this, NewLocation);
	SetNetLocation(NewLocation);
	CurrentTile = NewCurrent;
	MARK_PROPERTY_DIRTY_FROM_NAME(ACivilian, CurrentTile, this)
}

bool ACivilian::IsPathValid()
{
	return !Path.IsEmpty() && Path[Path.Num() - 1]->AcceptsCivilian();
}
