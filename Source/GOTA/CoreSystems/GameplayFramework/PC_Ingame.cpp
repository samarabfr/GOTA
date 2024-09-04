// Fill out your copyright notice in the Description page of Project Settings.

#include "PC_Ingame.h"
#include "Net/UnrealNetwork.h"

void APC_Ingame::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	
	DOREPLIFETIME(APC_Ingame, Guardian)
	DOREPLIFETIME(APC_Ingame, MouseUtils)
}

void APC_Ingame::BeginPlay()
{
	Super::BeginPlay();
	if (!IsLocalController()) return;
	CreateLobbyUI();
	DistanceUtils = GetWorld()->SpawnActor<ADistanceUtils>();
}

void APC_Ingame::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
	SetGuardian(Cast<AGuardian>(InPawn));
}

// ---------------------------------------------------------
// Getter & Setter

AGuardian* APC_Ingame::GetGuardian()
{
	return Guardian;
}

void APC_Ingame::SetGuardian(AGuardian* Guardian_)
{
	Guardian = Guardian_;
	GuardianChanged();
}

void APC_Ingame::OnRep_Guardian()
{
	GuardianChanged();
}

void APC_Ingame::GuardianChanged()
{
	if (!Guardian) return;
	if (!IsLocalController()) return;
	DistanceUtils->AttachToActor(Guardian, FAttachmentTransformRules::SnapToTargetIncludingScale);
	if(MouseUtils) Guardian->SetupGAM(MouseUtils);
}

AMouseUtils* APC_Ingame::GetMouseUtils()
{
	return MouseUtils;
}

void APC_Ingame::SetMouseUtils(AMouseUtils* MouseUtils_)
{
	MouseUtils = MouseUtils_;
	MouseUtilsChanged();
}

void APC_Ingame::OnRep_MouseUtils()
{
	MouseUtilsChanged();
}

void APC_Ingame::MouseUtilsChanged()
{
	if (IsLocalController())
	{
		MouseUtils->SetPlayerController(this);
		BindToMouseUtils(MouseUtils);
		if(Guardian) Guardian->SetupGAM(MouseUtils);
	}
}
