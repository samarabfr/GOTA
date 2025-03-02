// Fill out your copyright notice in the Description page of Project Settings.


#include "Entity.h"

#include "NiagaraComponent.h"
#include "Components/SplineComponent.h"
#include "Components/StateTreeComponent.h"
#include "GOTA/CoreSystems/Faction/Settlement/Settlement.h"
#include "GOTA/CoreSystems/GameplayFramework/GS_Ingame.h"
#include "GOTA/CoreSystems/Tile/Tile.h"
#include "Net/UnrealNetwork.h"
#include "Net/Core/PushModel/PushModel.h"

// ----------------------- Replication Setup -----------------------

void AEntity::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	FDoRepLifetimeParams Params;
	Params.bIsPushBased = true;

	Params.Condition = COND_InitialOnly;
	Params.RepNotifyCondition = REPNOTIFY_Always;
	DOREPLIFETIME_WITH_PARAMS(AEntity, OriginBuilding, Params);
	DOREPLIFETIME_WITH_PARAMS(AEntity, MovementRate, Params);
	DOREPLIFETIME_WITH_PARAMS(AEntity, Affiliation, Params);

	Params.Condition = COND_None;
	Params.RepNotifyCondition = REPNOTIFY_OnChanged;
	DOREPLIFETIME_WITH_PARAMS(AEntity, Progress, Params);
	DOREPLIFETIME_WITH_PARAMS(AEntity, CurrentTile, Params);
	DOREPLIFETIME_WITH_PARAMS(AEntity, NetLocation, Params);
	DOREPLIFETIME_WITH_PARAMS(AEntity, bProgresserActive, Params);
	DOREPLIFETIME_WITH_PARAMS(AEntity, ProgressRate, Params);
}

// ----------------------- LifeCycle -----------------------

AEntity::AEntity()
{
	bReplicates = true;
	bAlwaysRelevant = true;
	bReplicateUsingRegisteredSubObjectList = true;
	SetNetUpdateFrequency(0.1f);

	RootComponent = CreateDefaultSubobject<USceneComponent>("ROOT");
	StateTree = CreateDefaultSubobject<UStateTreeComponent>("StateTree");
	// Path graphics
	Spline = CreateDefaultSubobject<USplineComponent>("Spline Path");
	Spline->SetupAttachment(RootComponent);
	NiagaraPath = CreateDefaultSubobject<UNiagaraComponent>("Niagara Path");
	NiagaraPath->SetupAttachment(RootComponent);
}

void AEntity::S_HandleDeath()
{
	if (!GetCurrentTile()) return;
	GetCurrentTile()->RemoveEntity(this, GetEntityType());
}

void AEntity::Delete()
{
	S_HandleDeath();
	Destroy();
}

void AEntity::S_Init(UBuilding* InBuilding, ATile* SpawnTile)
{
	GameState = GetWorld()->GetGameState<AGS_Ingame>();
	OriginBuilding = InBuilding;
	OriginBuilding->OnEfficiencyChanged.AddDynamic(this, &AEntity::S_HandleEfficiencyChange);
	CurrentTile = SpawnTile;
	Affiliation = OriginBuilding->GetSettlement()->GetAffiliation();

	StateTree->StartLogic();
}

void AEntity::S_Tick(const float DeltaSeconds)
{
	if (bProgresserActive)
	{
		const float NewProgressRate = CalculateProgressRate();
		if (ProgressRate != NewProgressRate)
		{
			ProgressRate = NewProgressRate;
			MARK_PROPERTY_DIRTY_FROM_NAME(AEntity, ProgressRate, this)
			ForceNetUpdate();
		}
		ProgressTick(DeltaSeconds);
		if (Progress >= 100.f)
		{
			FinishProgress();
			Progress = 0.f;
			MARK_PROPERTY_DIRTY_FROM_NAME(AEntity, Progress, this)
			ForceNetUpdate();
		}
	}
}

void AEntity::C_Tick(const float DeltaSeconds)
{
	if (bProgresserActive)
	{
		ProgressTick(DeltaSeconds);
	}
}


// ----------------------- Utility -----------------------

void AEntity::OnRep_Affiliation()
{
}

UBuilding* AEntity::GetOriginBuilding() const
{
	return OriginBuilding.Get();
}

AGS_Ingame* AEntity::S_GetGameState() const
{
	return GameState.Get();
}

void AEntity::S_HandleEfficiencyChange(float Change)
{
}

EEntityType AEntity::GetEntityType() const
{
	return EEntityType::Other;
}

EAffiliation AEntity::GetAffiliation() const
{
	return Affiliation;
}

// ----------------- Progresser ------------------------

void AEntity::S_StartProgresser(const std::function<float()>& ProgressRateCalculator,
                                const std::function<void()>& Finisher)
{
	if (!ProgressRateCalculator || !Finisher) return;
	bProgresserActive = true;
	Progress = 0.f;
	MARK_PROPERTY_DIRTY_FROM_NAME(AEntity, bProgresserActive, this)
	MARK_PROPERTY_DIRTY_FROM_NAME(AEntity, Progress, this)
	CalculateProgressRate = ProgressRateCalculator;
	FinishProgress = Finisher;
	ProgressRate = CalculateProgressRate();
	MARK_PROPERTY_DIRTY_FROM_NAME(AEntity, ProgressRate, this)
	ForceNetUpdate();
}

void AEntity::S_StopProgresser()
{
	bProgresserActive = false;
	Progress = 0.f;
	MARK_PROPERTY_DIRTY_FROM_NAME(AEntity, bProgresserActive, this)
	MARK_PROPERTY_DIRTY_FROM_NAME(AEntity, Progress, this)
	CalculateProgressRate = nullptr;
	FinishProgress = nullptr;
	ProgressRate = 0.f;
	MARK_PROPERTY_DIRTY_FROM_NAME(AEntity, ProgressRate, this)
	ForceNetUpdate();
}

void AEntity::ProgressTick(float DeltaSeconds)
{
	if (Progress == 100.f) return;
	Progress += ProgressRate * DeltaSeconds;
	if (Progress > 100.f)
		Progress = 100.f;
}

// ----------------- Movement ------------------------


void AEntity::OnRep_NetLocation()
{
	SetActorLocation(NetLocation);
}

void AEntity::SetNetLocation(const FVector& NewNetLocation)
{
	SetActorLocation(NewNetLocation);
	NetLocation = NewNetLocation;
	MARK_PROPERTY_DIRTY_FROM_NAME(AEntity, NetLocation, this)
}

ATile* AEntity::GetCurrentTile() const
{
	return CurrentTile.Get();
}

void AEntity::S_SetPath(const TArray<ATile*>& NewPath)
{
	Path = NewPath;
}

void AEntity::S_SetMovementRate(float NewMovementRate)
{
	MovementRate = NewMovementRate;
}

void AEntity::S_MoveToNextTileOnPath()
{
	ATile* NewCurrent = nullptr;
	if (!Path.IsEmpty())
	{
		NewCurrent = Path.Pop();
	}
	if (!NewCurrent) return;
	CurrentTile->RemoveEntity(this, GetEntityType());
	FVector NewLocation = FVector();
	NewCurrent->AddEntity(this, GetEntityType(), NewLocation);
	SetNetLocation(NewLocation);
	CurrentTile = NewCurrent;
	MARK_PROPERTY_DIRTY_FROM_NAME(AEntity, CurrentTile, this)
}

float AEntity::GetMovementRate() const
{
	return MovementRate;
}

bool AEntity::IsPathValid()
{
	if (Path.IsEmpty()) return false;
	ATile* Goal = Path[Path.Num() - 1];
	if (!Goal) return false;
	return Goal->AcceptsEntity(GetEntityType());
}

bool AEntity::IsPathEmpty() const
{
	return Path.IsEmpty();
}

// ----------------- Path Graphics ------------------------

void AEntity::RefreshSpline()
{
	Spline->ClearSplinePoints(false);
	if (Path.Num() < 1)
	{
		Spline->UpdateSpline();
		NiagaraPath->SetHiddenInGame(true);
		return;
	}
	Spline->AddSplinePoint(CurrentTile->GetActorLocation() + FVector(0, 0, 300),
	                       ESplineCoordinateSpace::World, false);
	const int32 MaxSteps = 2 * MovementRate;
	for (int32 i = 0; i < Path.Num(); ++i)
	{
		if (i >= MaxSteps) break;
		Spline->AddSplinePoint(Path[Path.Num() - i - 1]->GetActorLocation() + FVector(0, 0, 300),
		                       ESplineCoordinateSpace::World, false);
	}
	Spline->UpdateSpline();
	NiagaraPath->SetHiddenInGame(false);
}
