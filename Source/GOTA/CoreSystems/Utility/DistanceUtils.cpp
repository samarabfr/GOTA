// Fill out your copyright notice in the Description page of Project Settings.


#include "DistanceUtils.h"

#include "GOTA/CoreSystems/GameplayFramework/PC_Ingame.h"
#include "GOTA/CoreSystems/Tile/HexCoordsFunctions.h"

ADistanceUtils::ADistanceUtils()
{
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;
	PrimaryActorTick.TickInterval = 0.2f;

	// Load Settings DataAsset
	static ConstructorHelpers::FObjectFinder<UDistanceUtilsSettings> DataAsset2(
		TEXT("/Game/CoreSystems/Utility/DistanceUtilsSettings"));
	if (DataAsset2.Succeeded())
	{
		Settings = DataAsset2.Object;
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("DistanceUtils couldn't load Settings Data Asset"))
	}
	RootComponent = CreateDefaultSubobject<USceneComponent>("ROOT");
}

void ADistanceUtils::BeginPlay()
{
	Super::BeginPlay();
	GameState = GetWorld()->GetGameState<AGS_Ingame>();
	CurrentCoords = FHexCoords(-100, -100);
}

void ADistanceUtils::Tick(float DeltaSeconds)
{
	// TileMap doesn't exist yet. Idk why this should ever happen but it did. LoadingManager should prevent this
	if (!GameState->TileMap) return;
	FVector CurrentLocation = GetActorLocation();
	FHexCoords NewCoords = UHexCoordsFunctions::VectorToHexCoords(CurrentLocation);
	if (CurrentCoords == NewCoords) return;
	// We are on a New Tile
	CurrentCoords = NewCoords;
	// UpdateDistanceToTiles();
	UpdateDistanceToCombats();
}

void ADistanceUtils::UpdateDistanceToTiles()
{
	TArray<FHexCoords> NextCoordsInRange
		= UHexCoordsFunctions::GetAllCoordsInRange(CurrentCoords, Settings->ActiveTileRangeInTiles);
	for (FHexCoords Coords : NextCoordsInRange)
	{
		if (!CurrentCoordsInRange.Contains(Coords))
		{
			if (ATile* Tile = GameState->TileMap->GetTile(Coords))
			{
				Tile->OnEnteringActiveRangeOfGuardian();
			}
		}
	}
	for (FHexCoords Coords : CurrentCoordsInRange)
	{
		if (!NextCoordsInRange.Contains(Coords))
		{
			if (ATile* Tile = GameState->TileMap->GetTile(Coords))
			{
				Tile->OnLeavingActiveRangeOfGuardian();
			}
		}
	}
	CurrentCoordsInRange = NextCoordsInRange;
}

void ADistanceUtils::UpdateDistanceToCombats()
{
	float ClosestDistance = MAX_flt;
	ACombat* ClosestCombat = nullptr;
	for (ACombat* Combat : GameState->CombatSystem->Combats)
	{
		float Distance = FVector::Distance(GetActorLocation(), Combat->GetActorLocation());
		if (Distance < ClosestDistance)
		{
			ClosestDistance = Distance;
			ClosestCombat = Combat;
		}
	}
	if (ClosestDistance > Settings->MinDistanceToCombatInCM)
	{
		ClosestCombat = nullptr;
	}
	GetWorld()->GetFirstPlayerController<APC_Ingame>()->WatchCombat(ClosestCombat);
}
