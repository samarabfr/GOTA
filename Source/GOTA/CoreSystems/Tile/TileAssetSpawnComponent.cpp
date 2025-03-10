// Fill out your copyright notice in the Description page of Project Settings.


#include "TileAssetSpawnComponent.h"

UTileAssetSpawnComponent::UTileAssetSpawnComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

FSpawnPoint UTileAssetSpawnComponent::GetSpawnPoint()
{
	FSpawnPoint SpawnPoint = FSpawnPoint();
	SpawnPoint.LocationOnTile = GetRelativeLocation();
	SpawnPoint.Rotation = GetRelativeRotation().Yaw;
	SpawnPoint.ForcedAssets = ForcedAssets;
	SpawnPoint.SpawnChance = SpawnChance;
	return SpawnPoint;
}
