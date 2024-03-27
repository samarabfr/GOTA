// Fill out your copyright notice in the Description page of Project Settings.


#include "GOTAGameState.h"

AGOTAGameState::AGOTAGameState()
{
	bReplicates = true;
	GOTAGameState = this;
}

AGOTAGameState* AGOTAGameState::GOTAGameState = nullptr;

AGOTAGameState* AGOTAGameState::GetGOTAGameState()
{
	return GOTAGameState;
}