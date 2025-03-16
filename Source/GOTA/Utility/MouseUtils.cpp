// Fill out your copyright notice in the Description page of Project Settings.


#include "MouseUtils.h"

#include "GOTA/Entity/Army.h"
#include "GOTA/Entity/Civilian.h"
#include "GOTA/GameplayFramework/GS_Ingame.h"
#include "GOTA/GameplayFramework/PC_Ingame.h"
#include "GOTA/Guardian/AbilityFramework/AbilityTarget.h"
#include "GOTA/Guardian/Guardian.h"
#include "GOTA/Tile/Tile.h"
#include "Net/UnrealNetwork.h"
#include "Net/Core/PushModel/PushModel.h"

void AMouseUtils::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	FDoRepLifetimeParams Params;
	Params.bIsPushBased = false;
	Params.Condition = COND_SkipOwner;
	Params.RepNotifyCondition = REPNOTIFY_OnChanged;
	DOREPLIFETIME_WITH_PARAMS(AMouseUtils, NetMouseLocation, Params)

	Params.bIsPushBased = true;
	Params.Condition = COND_SkipOwner;
	Params.RepNotifyCondition = REPNOTIFY_OnChanged;
	DOREPLIFETIME_WITH_PARAMS(AMouseUtils, HoverTile, Params)
}

// ------------------ LifeCycle ------------------

AMouseUtils::AMouseUtils()
{
	bReplicates = true;
	bAlwaysRelevant = true;

	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = false;
	PrimaryActorTick.TickInterval = 0.0f;

	RootComponent = CreateDefaultSubobject<USceneComponent>("ROOT");
	MouseLocation = CreateDefaultSubobject<USceneComponent>("Mouse Location");
	MouseLocation->SetupAttachment(RootComponent);
	MouseTileLocation = CreateDefaultSubobject<USceneComponent>("Mouse Tile Location");
	MouseTileLocation->SetupAttachment(RootComponent);

	TestCube = CreateDefaultSubobject<UStaticMeshComponent>("TestCube");
	TestCube->SetupAttachment(MouseLocation);
	TestCube->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void AMouseUtils::BeginPlay()
{
	Super::BeginPlay();
	TestCube->SetStaticMesh(TestCubeMesh);
	GetWorld()->GetGameState<AGS_Ingame>()->IncrementReplicationCount();

	if (IsOwnedBy(GetWorld()->GetFirstPlayerController()))
	{
		SetActorTickEnabled(true);
	}
}

void AMouseUtils::Tick(float DeltaSeconds)
{
	FHitResult HitResult;
	if (PlayerController && PlayerController->GetHitResultUnderCursor(ECC_Visibility, false, HitResult))
	{
		SetMouseLocation(HitResult.Location);

		ATile* HitTile = Cast<ATile>(HitResult.GetActor());
		if (HitTile && HoverTile != HitTile)
		{
			SetHoverTile(HitTile);
		}

		if (HitResult.GetActor() != HoverActor)
		{
			HoverActor = HitResult.GetActor();
			OnHoverActorChanged.Broadcast(HoverActor);
		}
	}
}

void AMouseUtils::SetPlayerController(APC_Ingame* PC)
{
	PlayerController = PC;
}

// ------------------ Mouse Location ------------------

void AMouseUtils::SetMouseLocation(const FVector Location)
{
	SRPC_SetNetMouseLocation(Location);
	if (!HasAuthority())
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

// ------------------ Hover Tile ------------------

void AMouseUtils::SetHoverTile(ATile* NewTile)
{
	if (NewTile == HoverTile) return;
	SRPC_SetHoverTile(NewTile);
	if (!HasAuthority())
	{
		// if the server calls this, it does this in the RPC
		HoverTile = NewTile;
		HoverTileChanged();
	}
}

void AMouseUtils::SRPC_SetHoverTile_Implementation(ATile* NewTile)
{
	HoverTile = NewTile;
	MARK_PROPERTY_DIRTY_FROM_NAME(AMouseUtils, HoverTile, this);
	HoverTileChanged();
}

void AMouseUtils::OnRep_HoverTile()
{
	HoverTileChanged();
}

void AMouseUtils::HoverTileChanged()
{
	if (HoverTile)
	{
		MouseTileLocation->SetRelativeLocation(HoverTile->GetActorLocation());
	}
	OnHoverTileChanged.Broadcast(HoverTile);
}

void AMouseUtils::AttachActorToTilePosition(AActor* Actor)
{
	Actor->AttachToComponent(MouseTileLocation, FAttachmentTransformRules::SnapToTargetIncludingScale);
}

FAbilityTarget AMouseUtils::GetHoverAbilityTarget() const
{
	FAbilityTarget AbilityTarget;
	AbilityTarget.Tile = Cast<ATile>(HoverActor);
	AbilityTarget.Guardian = Cast<AGuardian>(HoverActor);
	AbilityTarget.Civilian = Cast<ACivilian>(HoverActor);
	AbilityTarget.Army = Cast<AArmy>(HoverActor);
	return AbilityTarget;
}

// ------------------ Hover Actor ------------------
