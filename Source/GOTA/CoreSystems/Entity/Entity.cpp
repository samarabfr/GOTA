// Fill out your copyright notice in the Description page of Project Settings.


#include "Entity.h"
#include "GOTA/CoreSystems/Tile/Tile.h"
#include "Net/UnrealNetwork.h"

void AEntity::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AEntity, Path);
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

TArray<ATile*> AEntity::GetPath()
{
	return Path;
}

void AEntity::SetPath(const TArray<ATile*>& NewPath)
{
	Path = NewPath;
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
