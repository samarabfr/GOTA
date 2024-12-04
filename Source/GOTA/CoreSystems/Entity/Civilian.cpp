#include "Civilian.h"
#include "CivilianSettings.h"
#include "GOTA/CoreSystems/Faction/Building/Building.h"
#include "GOTA/CoreSystems/Faction/Building/BuildingSettings.h"
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
	DOREPLIFETIME_WITH_PARAMS(ACivilian, Building, Params);
	DOREPLIFETIME_WITH_PARAMS(ACivilian, Settings, Params);
	DOREPLIFETIME_WITH_PARAMS(ACivilian, WorkAmount, Params);
	DOREPLIFETIME_WITH_PARAMS(ACivilian, MovementRate, Params);

	Params.Condition = COND_None;
	Params.RepNotifyCondition = REPNOTIFY_OnChanged;
	DOREPLIFETIME_WITH_PARAMS(ACivilian, Progress, Params);
	DOREPLIFETIME_WITH_PARAMS(ACivilian, CurrentTile, Params);
	DOREPLIFETIME_WITH_PARAMS(ACivilian, NetLocation, Params);
	DOREPLIFETIME_WITH_PARAMS(ACivilian, bProgresserActive, Params);
	DOREPLIFETIME_WITH_PARAMS(ACivilian, ProgressRate, Params);
	DOREPLIFETIME_WITH_PARAMS(ACivilian, PriorityTile, Params);
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
	NetUpdateFrequency = 1.0f;

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
	Building = InBuilding;
	CurrentTile = SpawnTile;
	FVector NewLocation = FVector();
	SpawnTile->AddCivilian(this, NewLocation);
	SetNetLocation(NewLocation);

	const UBuildingSettings* BuildingSettings = Building->GetSettings();
	WorkAmount = BuildingSettings->CivilianProductionAmount;
	MovementRate = 100 / BuildingSettings->CivilianMoveTime;

	StateTree->StartLogic();
}

void ACivilian::S_Tick(const float DeltaSeconds)
{
	if (bProgresserActive)
	{
		ProgressRate = CalculateProgressRate();
		MARK_PROPERTY_DIRTY_FROM_NAME(ACivilian, ProgressRate, this)
		TickProgress(DeltaSeconds);
		MARK_PROPERTY_DIRTY_FROM_NAME(ACivilian, Progress, this)
		if (Progress >= 100.f)
		{
			FinishProgress();
			Progress = 0.f;
			MARK_PROPERTY_DIRTY_FROM_NAME(ACivilian, Progress, this)
		}
	}
}

void ACivilian::C_Tick(const float DeltaSeconds)
{
	if (bProgresserActive)
	{
		TickProgress(DeltaSeconds);
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
}

void ACivilian::S_StopProgresser()
{
	bProgresserActive = false;
	Progress = 0.f;
	MARK_PROPERTY_DIRTY_FROM_NAME(ACivilian, bProgresserActive, this)
	MARK_PROPERTY_DIRTY_FROM_NAME(ACivilian, Progress, this)
	CalculateProgressRate = nullptr;
	FinishProgress = nullptr;
}

void ACivilian::TickProgress(float DeltaSeconds)
{
	Progress += ProgressRate * DeltaSeconds;
}

// ----------------- Working ------------------------

bool ACivilian::IsTileValidForWork(const ATile* Tile) const
{
	return false;
}

void ACivilian::S_Work()
{
}

float ACivilian::GetWorkRate() const
{
	return 100.f / Building->GetSettings()->CivilianProductionTime * Building->GetEfficiency();
}

bool ACivilian::IsCurrentTileValidForWork() const
{
	if (!CurrentTile.IsValid()) return false;
	return IsTileValidForWork(GetCurrentTile());
}

bool ACivilian::IsPriorityTileValidForWork() const
{
	if (!PriorityTile.IsValid()) return false;
	return IsTileValidForWork(GetPriorityTile());
}

bool ACivilian::TryFindPathToNearestTileValidForWork()
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

bool ACivilian::TryFindPathToPriorityTile()
{
	if (!CurrentTile.IsValid() || !PriorityTile.IsValid()) return false;
	Path = GameState->GetTileMap()->GetPath(GetCurrentTile(), GetPriorityTile(), EEntityType::Civilian);
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
