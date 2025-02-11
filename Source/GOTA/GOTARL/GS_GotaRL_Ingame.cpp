// Fill out your copyright notice in the Description page of Project Settings.


#include "GS_GotaRL_Ingame.h"

#include "SimulatedGuardianManager.h"

ASimulatedGuardianManager* AGS_GotaRL_Ingame::GetLearningManager()
{
	return LearningManager.Get();
}

void AGS_GotaRL_Ingame::SetLearningManager(ASimulatedGuardianManager* NewLearningManager)
{
	LearningManager = NewLearningManager;
}
