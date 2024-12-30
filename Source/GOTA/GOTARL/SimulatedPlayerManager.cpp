// Fill out your copyright notice in the Description page of Project Settings.


#include "SimulatedPlayerManager.h"

#include "LearningAgentsManager.h"
#include "Net/UnrealNetwork.h"

ASimulatedPlayerManager::ASimulatedPlayerManager()
{
	bReplicates = true;
	bAlwaysRelevant = true;
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;
	
	RootComponent = CreateDefaultSubobject<USceneComponent>("ROOT");
	ManagerComponent = CreateDefaultSubobject<ULearningAgentsManager>("StateTree");

	Tags.Add("LearningAgentsManager");
}

void ASimulatedPlayerManager::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	FDoRepLifetimeParams Params;
	Params.bIsPushBased = true;

	Params.Condition = COND_InitialOnly;
	Params.RepNotifyCondition = REPNOTIFY_Always;

	Params.Condition = COND_None;
	Params.RepNotifyCondition = REPNOTIFY_OnChanged;
}

void ASimulatedPlayerManager::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	if (HasAuthority())
		S_Tick(DeltaSeconds);
	else
		C_Tick(DeltaSeconds);
}

void ASimulatedPlayerManager::S_Init()
{
}

void ASimulatedPlayerManager::S_Tick(const float DeltaSeconds)
{
}

void ASimulatedPlayerManager::C_Tick(const float DeltaSeconds)
{
}

void ASimulatedPlayerManager::BeginDestroy()
{
	Super::BeginDestroy();
}

void ASimulatedPlayerManager::RegisterAgent(UObject* Agent)
{
	if(!ManagerComponent || !Agent) return;
	ManagerComponent->AddAgent(Agent);
}
