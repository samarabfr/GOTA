// Fill out your copyright notice in the Description page of Project Settings.


#include "PC_Ingame.h"

void APC_Ingame::BeginPlay()
{
	Super::BeginPlay();
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
}

void APC_Ingame::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
	if (!IsLocalController()) return;
	DistanceUtils->AttachToActor(Guardian, FAttachmentTransformRules::SnapToTargetIncludingScale);
}

AGuardian* APC_Ingame::GetGuardian()
{
	return Guardian;
}
