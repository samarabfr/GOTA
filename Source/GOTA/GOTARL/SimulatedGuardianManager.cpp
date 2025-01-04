// Fill out your copyright notice in the Description page of Project Settings.


#include "SimulatedGuardianManager.h"

#include "LearningAgentsManager.h"
#include "Net/UnrealNetwork.h"

ASimulatedGuardianManager::ASimulatedGuardianManager()
{
	bReplicates = true;
	bAlwaysRelevant = true;
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;
	
	RootComponent = CreateDefaultSubobject<USceneComponent>("ROOT");
	ManagerComponent = CreateDefaultSubobject<ULearningAgentsManager>("StateTree");

	Tags.Add("LearningAgentsManager");
}

void ASimulatedGuardianManager::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	FDoRepLifetimeParams Params;
	Params.bIsPushBased = true;

	Params.Condition = COND_InitialOnly;
	Params.RepNotifyCondition = REPNOTIFY_Always;

	Params.Condition = COND_None;
	Params.RepNotifyCondition = REPNOTIFY_OnChanged;
}

void ASimulatedGuardianManager::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	if (HasAuthority())
		S_Tick(DeltaSeconds);
	else
		C_Tick(DeltaSeconds);
}

void ASimulatedGuardianManager::S_Init()
{
}

void ASimulatedGuardianManager::S_Tick(const float DeltaSeconds)
{
}

void ASimulatedGuardianManager::C_Tick(const float DeltaSeconds)
{
}

void ASimulatedGuardianManager::BeginDestroy()
{
	Super::BeginDestroy();
}

void ASimulatedGuardianManager::RegisterAgent(UObject* Agent)
{
	if(!ManagerComponent || !Agent) return;
	ManagerComponent->AddAgent(Agent);
}
