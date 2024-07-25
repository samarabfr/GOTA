// Fill out your copyright notice in the Description page of Project Settings.


#include "TileEntity.h"
#include "Settlement.h"
#include "GOTA/Core/GOTAGameState.h"
#include "GOTA/TileMap/Tile.h"
#include "Net/UnrealNetwork.h"

void ATileEntity::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ATileEntity, Affiliation);
	DOREPLIFETIME(ATileEntity, Target);
	DOREPLIFETIME(ATileEntity, CurrentTile);
	DOREPLIFETIME(ATileEntity, Path);
	DOREPLIFETIME(ATileEntity, MovementSpeed);
}

ATileEntity::ATileEntity()
{
	bReplicates = true;
}

void ATileEntity::Init(EAffiliation Affiliation_, ATile* CurrentTile_, int32 MovementSpeed_)
{
	Affiliation = Affiliation_;
	CurrentTile = CurrentTile_;
	MovementSpeed = MovementSpeed_;
	SetActorLocation(CurrentTile->GetActorLocation());
	AGOTAGameState* GameState = GetWorld()->GetGameState<AGOTAGameState>();
	GameState->TileEntities.Add(this);
}

bool ATileEntity::ShouldCombatTrigger() const
{
	// Combat between this unit and enemy building
	if (CurrentTile->Building
		&& CurrentTile->GetClaimant()
		&& CurrentTile->GetClaimant()->Affiliation != Affiliation)
	{
		return true;
	}
	// Combat between this unit and enemy entity
	if (Affiliation == EAffiliation::Ally)
	{
		if (CurrentTile->EnemyTileEntity) return true;
	}
	else
	{
		if (CurrentTile->AlliedTileEntity) return true;
	}
	// no combat
	return false;
}

void ATileEntity::TriggerCombat()
{
	// destroy building
	if (CurrentTile->Building) CurrentTile->Unbuild();
	// destroy enemy Entity
	if (Affiliation == EAffiliation::Ally)
	{
		if (CurrentTile->EnemyTileEntity) CurrentTile->EnemyTileEntity->Kill();
	}
	else
	{
		if (CurrentTile->AlliedTileEntity) CurrentTile->AlliedTileEntity->Kill();
	}
	// destroy this entity
	Kill();
}

void ATileEntity::Kill()
{
	AGOTAGameState* GameState = GetWorld()->GetGameState<AGOTAGameState>();
	GameState->TileEntities.Remove(this);
	if (Affiliation == EAffiliation::Ally)
	{
		CurrentTile->AlliedTileEntity = nullptr;
	}
	else
	{
		CurrentTile->EnemyTileEntity = nullptr;
	}
	Destroy();
}

bool ATileEntity::IsNextStepBlocked()
{
	for (int32 i = 0; i < MovementSpeed; ++i)
	{
		int32 index = Path.Num() -1 -i;
		if(Path.IsValidIndex(index))
		{
			if(!Path[index]->IsWalkable(Affiliation)) return true;
		}
	}
	return false;
}

void ATileEntity::Step()
{
	ATile* NewCurrent = nullptr;
	for (int32 i = 0; i < MovementSpeed; ++i)
	{
		if(!Path.IsEmpty())
		{
			NewCurrent = Path.Pop();
		}
	}
	// can't move
	if(!NewCurrent) return;
	// move
	if (Affiliation == EAffiliation::Ally)
	{
		CurrentTile->AlliedTileEntity = nullptr;
		NewCurrent->AlliedTileEntity = this;
	}
	else
	{
		CurrentTile->EnemyTileEntity = nullptr;
		NewCurrent->EnemyTileEntity = this;
	}
	CurrentTile=NewCurrent;
	SetActorLocation(CurrentTile->GetActorLocation());
}
