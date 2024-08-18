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
	SetReplicateMovement(true);
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

int32 AEntity::GetAttack() const
{
	return 0;
}

int32 AEntity::GetDefense() const
{
	return 0;
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

void AEntity::TriggerCombat()
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

void AEntity::Kill()
{
	AGS_Ingame* GameState = GetWorld()->GetGameState<AGS_Ingame>();
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

bool AEntity::IsNextStepBlocked()
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

void AEntity::Step()
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
