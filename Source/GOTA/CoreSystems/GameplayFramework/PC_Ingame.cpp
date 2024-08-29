// Fill out your copyright notice in the Description page of Project Settings.


#include "PC_Ingame.h"

void APC_Ingame::BeginPlay()
{
	if (!IsLocalController()) return;
	CreateLobbyUI();
	DistanceUtils = GetWorld()->SpawnActor<ADistanceUtils>();
}

void APC_Ingame::PossessGuardian(AGuardian* NewGuardian)
{
	Possess(NewGuardian);
	Guardian = NewGuardian;
	Guardian->GetMouseUtils()->SetPlayerController(this);
	BindToMouseUtils(Guardian->GetMouseUtils());
	DistanceUtils->AttachToActor(Guardian, FAttachmentTransformRules::SnapToTargetIncludingScale);
}

AGuardian* APC_Ingame::GetGuardian()
{
	return Guardian;
}
