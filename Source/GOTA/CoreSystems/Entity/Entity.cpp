// Fill out your copyright notice in the Description page of Project Settings.


#include "Entity.h"
#include "GOTA/CoreSystems/Faction/Settlement/Settlement.h"
#include "GOTA/CoreSystems/GameplayFramework/GS_Ingame.h"
#include "GOTA/CoreSystems/Tile/Tile.h"
#include "Net/UnrealNetwork.h"

void AEntity::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AEntity, Affiliation);
	DOREPLIFETIME(AEntity, Target);
	DOREPLIFETIME(AEntity, CurrentTile);
	DOREPLIFETIME(AEntity, Path);
	DOREPLIFETIME(AEntity, MovementSpeed);
}

AEntity::AEntity()
{
	bReplicates = true;
	AActor::SetReplicateMovement(true);
	RootComponent = CreateDefaultSubobject<USceneComponent>("ROOT");
	Spline = CreateDefaultSubobject<USplineComponent>("Spline Path");
	Spline->SetupAttachment(RootComponent);
	NiagaraPath = CreateDefaultSubobject<UNiagaraComponent>("Niagara Path");
	NiagaraPath->SetupAttachment(RootComponent);
}

void AEntity::CombatValuesChanged(UCombatValues* CombatValues)
{
	OnCombatValuesChanged.Broadcast(CombatValues);
}

EAffiliation AEntity::GetAffiliation()
{
	return Affiliation;
}

TArray<ATile*> AEntity::GetPath()
{
	return Path;
}

void AEntity::SetPath(const TArray<ATile*>& NewPath)
{
	Path = NewPath;
	RefreshSpline();
}

void AEntity::Init(EAffiliation Affiliation_, ATile* CurrentTile_, int32 MovementSpeed_)
{
	Affiliation = Affiliation_;
	CurrentTile = CurrentTile_;
	MovementSpeed = MovementSpeed_;
	SetActorLocation(CurrentTile->GetActorLocation());
	AGS_Ingame* GameState = GetWorld()->GetGameState<AGS_Ingame>();
	GameState->TileEntities.Add(this);
}

void AEntity::KillIndividuals(int32 Kills)
{
}

bool AEntity::ShouldCombatTrigger() const
{
	// Combat between this unit and enemy building
	if (CurrentTile->Building
		&& CurrentTile->GetClaimant()
		&& CurrentTile->GetClaimant()->Affiliation != Affiliation)
	{
		return true;
	}
	// Combat between this unit and enemy entity
	if (CurrentTile->GetEntity(!Affiliation)) return true;
	// no combat
	return false;
}

void AEntity::Kill()
{
	AGS_Ingame* GameState = GetWorld()->GetGameState<AGS_Ingame>();
	GameState->TileEntities.Remove(this);
	CurrentTile->SetEntity(nullptr, Affiliation);
	Destroy();
}

bool AEntity::IsNextStepBlocked()
{
	for (int32 i = 0; i < MovementSpeed; ++i)
	{
		int32 index = Path.Num() - 1 - i;
		if (Path.IsValidIndex(index))
		{
			if (!Path[index]->IsWalkable(Affiliation)) return true;
		}
	}
	return false;
}

void AEntity::Step()
{
	ATile* NewCurrent = nullptr;
	for (int32 i = 0; i < MovementSpeed; ++i)
	{
		if (!Path.IsEmpty())
		{
			NewCurrent = Path.Pop();
		}
	}
	// can't move
	if (!NewCurrent) return;
	// move
	CurrentTile->SetEntity(nullptr, Affiliation);
	NewCurrent->SetEntity(this, Affiliation);
	CurrentTile = NewCurrent;
	SetActorLocation(CurrentTile->GetActorLocation());
	RefreshSpline();
}

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
	int32 MaxSteps = 2 * MovementSpeed;
	for (int32 i = 0; i < Path.Num(); ++i)
	{
		if (i >= MaxSteps) break;
		Spline->AddSplinePoint(Path[Path.Num() - i - 1]->GetActorLocation() + FVector(0, 0, 300),
		                       ESplineCoordinateSpace::World, false);
	}
	Spline->UpdateSpline();
	NiagaraPath->SetHiddenInGame(false);
}

UCombatValues* AEntity::GetCombatValues() const
{
	return nullptr;
}
