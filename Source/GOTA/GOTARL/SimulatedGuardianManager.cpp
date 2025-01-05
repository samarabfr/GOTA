// Fill out your copyright notice in the Description page of Project Settings.


#include "SimulatedGuardianManager.h"

#include "LearningAgentsManager.h"
#include "SimulatedGuardian.h"
#include "Kismet/GameplayStatics.h"

ASimulatedGuardianManager::ASimulatedGuardianManager()
{
	bReplicates = true;
	bAlwaysRelevant = true;
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;
	
	RootComponent = CreateDefaultSubobject<USceneComponent>("ROOT");
	ManagerComponent = CreateDefaultSubobject<ULearningAgentsManager>("LearningAgentsManager");

	Tags.Add("LearningAgentsManager");
}

void ASimulatedGuardianManager::BeginPlay()
{
	Super::BeginPlay();
	TArray<AActor*> GuardianActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ASimulatedGuardian::StaticClass(), GuardianActors);
}

void ASimulatedGuardianManager::RegisterAgent(UObject* Agent)
{
	if(!ManagerComponent || !Agent) return;
	ManagerComponent->AddAgent(Agent);
}
