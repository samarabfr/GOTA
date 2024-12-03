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
	DOREPLIFETIME_WITH_PARAMS(ACivilian, Status, Params);
	DOREPLIFETIME_WITH_PARAMS(ACivilian, CurrentTile, Params);
	DOREPLIFETIME_WITH_PARAMS(ACivilian, NetLocation, Params);
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
	WorkRate = 100 / BuildingSettings->CivilianProductionTime;
	
	StateTree->StartLogic();
}

void ACivilian::S_Tick(const float DeltaSeconds)
{
	C_Tick(DeltaSeconds);
	if (Progress >= 100)
	{
		if (GetStatus() == ECivilianStatus::MovingToNextTile)
			S_MoveToNextTileOnPath();
		else if (GetStatus() == ECivilianStatus::Working)
			S_Work();
		SetStatus(ECivilianStatus::Idling);
		Progress = 0.0f;
		StateTree->SendStateTreeEvent(Settings->StateTreeCompletedTaskEventTag, FConstStructView(), FName(GetName()));
		MARK_PROPERTY_DIRTY_FROM_NAME(ACivilian, Progress, this)
	}
}

void ACivilian::C_Tick(const float DeltaSeconds)
{
	if (GetStatus() == ECivilianStatus::MovingToNextTile)
	{
		Progress += MovementRate * DeltaSeconds;
	}
	else if (GetStatus() == ECivilianStatus::Working)
	{
		Progress += WorkRate * DeltaSeconds / Building->GetEfficiency();
	}
}

void ACivilian::BeginDestroy()
{
	Super::BeginDestroy();
	S_HandleDeath();
}

void ACivilian::S_HandleDeath()
{
	if (!CurrentTile) return;
	CurrentTile->RemoveCivilian(this);
	Destroy();
}

// ----------------------- Status -----------------------

void ACivilian::SetStatus(const ECivilianStatus NewStatus)
{
	if (Status == NewStatus) return;
	Status = NewStatus;
	Progress = 0.0f;
	MARK_PROPERTY_DIRTY_FROM_NAME(ACivilian, Status, this)
	MARK_PROPERTY_DIRTY_FROM_NAME(ACivilian, Progress, this)
}

// ----------------- Working ------------------------

bool ACivilian::IsTileValidForWork(const ATile* Tile) const
{
	return false;
}

void ACivilian::S_Work()
{
}

void ACivilian::S_StartWorking()
{
	Progress = 0.f;
	SetStatus(ECivilianStatus::Working);
}

bool ACivilian::IsCurrentTileValidForWork() const
{
	return IsTileValidForWork(CurrentTile);
}

bool ACivilian::TryFindPathToNearestTileValidForWork()
{
	if (!Building) return false;
	Path = GameState->GetTileMap()->FindPathToNearestTile(CurrentTile, EEntityType::Civilian, [this](const ATile* Tile)
	{
		return IsTileValidForWork(Tile);
	});
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

void ACivilian::S_StartMoveToNextTileOnPath()
{
	Progress = 0.f;
	SetStatus(ECivilianStatus::MovingToNextTile);
}
