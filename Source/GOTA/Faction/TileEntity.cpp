// Fill out your copyright notice in the Description page of Project Settings.


#include "TileEntity.h"
#include "Net/UnrealNetwork.h"

void ATileEntity::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ATileEntity, Target);
	DOREPLIFETIME(ATileEntity, CurrentTile);
	DOREPLIFETIME(ATileEntity, Path);
}
