// Fill out your copyright notice in the Description page of Project Settings.


#include "MouseUtils.h"
#include "GOTA/CoreSystems/GameplayFramework/PC_Ingame.h"
#include "Net/UnrealNetwork.h"

void AMouseUtils::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AMouseUtils, NetLocation)
	DOREPLIFETIME(AMouseUtils, NetTileLocation)
}

AMouseUtils::AMouseUtils()
{
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;
	bReplicates = true;
	bAlwaysRelevant = true;

	RootComponent = CreateDefaultSubobject<USceneComponent>("ROOT");
	MouseLocation = CreateDefaultSubobject<USceneComponent>("Mouse Location");
	MouseLocation->SetupAttachment(RootComponent);
	MouseTileLocation = CreateDefaultSubobject<USceneComponent>("Mouse Tile Location");
	MouseTileLocation->SetupAttachment(RootComponent);
}

void AMouseUtils::Tick(float DeltaSeconds)
{
	if (!PlayerController)return;
	if (!PlayerController->IsLocalController())return;

	FHitResult HitResult;
	if (PlayerController->GetHitResultUnderCursor(ECC_Visibility, false, HitResult))
	{
		SetNetLocation(HitResult.Location);
		MouseLocation->SetWorldLocation(HitResult.Location);

		ATile* HitTile = Cast<ATile>(HitResult.GetActor());
		if (HitTile && HoverTile != HitTile)
		{
			HoverTile = HitTile;
			MouseTileLocation->SetWorldLocation(HitTile->GetActorLocation());
			SetNetTileLocation(HitTile->GetActorLocation());
			OnHoverTileChanged.Broadcast(HitTile);
		}
	}
}

ATile* AMouseUtils::GetHoverTile() const
{
	return HoverTile;
}

FVector AMouseUtils::GetMouseLocation() const
{
	return MouseLocation->GetRelativeLocation();
}

FVector AMouseUtils::GetMouseTileLocation() const
{
	return MouseTileLocation->GetRelativeLocation();
}

void AMouseUtils::SetPlayerController(APC_Ingame* PC)
{
	PlayerController = PC;
}

void AMouseUtils::AttachToTilePosition(AActor* Actor)
{
	Actor->AttachToComponent(MouseTileLocation, FAttachmentTransformRules::SnapToTargetIncludingScale);
}

// ---------------------------------------------------------
// Replicated Locations to prevent Lag on MouseUtils

void AMouseUtils::OnRep_NetLocation()
{
	// ignore updates on its own MouseUtils because its kinda ClientSide
	if (PlayerController && PlayerController->IsLocalController()) return;
	MouseLocation->SetWorldLocation(NetTileLocation);
}

void AMouseUtils::SetNetLocation_Implementation(FVector Location)
{
	NetLocation = Location;
}

void AMouseUtils::OnRep_NetTileLocation()
{
	// ignore updates on its own MouseUtils because its kinda ClientSide
	if (PlayerController && PlayerController->IsLocalController()) return;
	MouseTileLocation->SetWorldLocation(NetTileLocation);
}

void AMouseUtils::SetNetTileLocation_Implementation(FVector Location)
{
	NetTileLocation = Location;
}
