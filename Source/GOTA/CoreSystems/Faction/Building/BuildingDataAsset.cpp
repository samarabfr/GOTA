// Fill out your copyright notice in the Description page of Project Settings.


#include "BuildingDataAsset.h"

FBuildingTierData* UBuildingDataAsset::GetTierData(int32 Tier)
{
	if(Tier == 1) return &TierOne;
	if(Tier == 2) return &TierTwo;
	if(Tier == 3) return &TierThree;
	return nullptr;
}