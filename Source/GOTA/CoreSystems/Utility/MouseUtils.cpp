// Fill out your copyright notice in the Description page of Project Settings.


#include "MouseUtils.h"

#include "GOTA/CoreSystems/GameplayFramework/LoadingManager.h"
#include "GOTA/CoreSystems/GameplayFramework/PC_Ingame.h"
#include "Net/UnrealNetwork.h"

void AMouseUtils::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	FDoRepLifetimeParams Params;
	Params.bIsPushBased = false;
	
	Params.Condition = COND_SkipOwner;
	Params.RepNotifyCondition = REPNOTIFY_OnChanged;
	DOREPLIFETIME_WITH_PARAMS(AMouseUtils, NetMouseLocation, Params)
	DOREPLIFETIME_WITH_PARAMS(AMouseUtils, NetMouseTileLocation, Params)
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

void AMouseUtils::BeginPlay()
{
	Super::BeginPlay();
	AGS_Ingame* GameState = GetWorld()->GetGameState<AGS_Ingame>();
	GameState->LoadingManager->IncrementReplicationCount();
}

void AMouseUtils::Tick(float DeltaSeconds)
{
	if (!PlayerController)return;
	if (!PlayerController->IsLocalController())return;

	FHitResult HitResult;
	if (PlayerController->GetHitResultUnderCursor(ECC_Visibility, false, HitResult))
	{
		SetMouseLocation(HitResult.Location);
		
		ATile* HitTile = Cast<ATile>(HitResult.GetActor());
		if (HitTile && HoverTile != HitTile)
		{
			HoverTile = HitTile;
			SetMouseTileLocation(HitTile->GetActorLocation());
			OnHoverTileChanged.Broadcast(HitTile);
		}

		if(HitResult.GetActor() != HoverActor)
		{
			HoverActor = HitResult.GetActor();
			OnHoverActorChanged.Broadcast(HoverActor);
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

void AMouseUtils::AttachActorToTilePosition(AActor* Actor)
{
	Actor->AttachToComponent(MouseTileLocation, FAttachmentTransformRules::SnapToTargetIncludingScale);
}

// ------------------ Mouse Location ------------------

void AMouseUtils::SetMouseLocation(const FVector Location)
{
	SRPC_SetNetMouseLocation(Location);
	if(!HasAuthority())
	{
		// if the server calls this, it does this in the RPC
		MouseLocation->SetWorldLocation(Location);
	}
}

void AMouseUtils::SRPC_SetNetMouseLocation_Implementation(const FVector Location)
{
	NetMouseLocation = Location;
	MouseLocation->SetWorldLocation(Location);
}

void AMouseUtils::OnRep_NetMouseLocation()
{
	MouseLocation->SetWorldLocation(NetMouseLocation);
}

// ------------------ Mouse Tile Location ------------------

void AMouseUtils::SetMouseTileLocation(const FVector Location)
{
	SRPC_SetNetMouseTileLocation(Location);
	if(!HasAuthority())
	{
		// if the server calls this, it does this in the RPC
		MouseTileLocation->SetWorldLocation(Location);
	}
}

void AMouseUtils::SRPC_SetNetMouseTileLocation_Implementation(const FVector Location)
{
	NetMouseTileLocation = Location;
	MouseTileLocation->SetWorldLocation(Location);
}

void AMouseUtils::OnRep_NetMouseTileLocation()
{
	MouseTileLocation->SetWorldLocation(NetMouseTileLocation);
}