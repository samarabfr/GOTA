// Fill out your copyright notice in the Description page of Project Settings.


#include "GS_Ingame.h"
#include "Net/UnrealNetwork.h"

//Unreal Engine Mystery Code
void AGS_Ingame::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AGS_Ingame, TileMap);

	DOREPLIFETIME(AGS_Ingame, TotalTrees);
	DOREPLIFETIME(AGS_Ingame, TotalForage);
	DOREPLIFETIME(AGS_Ingame, TotalWildlife);
	DOREPLIFETIME(AGS_Ingame, IslandMaxTrees);
	DOREPLIFETIME(AGS_Ingame, IslandMaxWildlife);
	DOREPLIFETIME(AGS_Ingame, IslandMaxForage);

	DOREPLIFETIME(AGS_Ingame, CombatSystem);
	DOREPLIFETIME(AGS_Ingame, StartParameter);
}

AGS_Ingame::AGS_Ingame()
{
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;
	bReplicateUsingRegisteredSubObjectList = true;
	TotalTrees = CreateDefaultSubobject<UGOTAAttribute>(TEXT("Total Trees"));
	TotalForage = CreateDefaultSubobject<UGOTAAttribute>(TEXT("Total Forage"));
	TotalWildlife = CreateDefaultSubobject<UGOTAAttribute>(TEXT("Total Wildlife"));
	TotalColonialPopulation = CreateDefaultSubobject<UTotalPopulation>(TEXT("Total Colonial Population"));
	TotalNativePopulation = CreateDefaultSubobject<UTotalPopulation>(TEXT("Total Native Population"));
	CombatSystem = CreateDefaultSubobject<UCombatSystem>(TEXT("Combat System"));
	StartParameter = CreateDefaultSubobject<UStartParameter>(TEXT("Start Parameter"));
}

void AGS_Ingame::BeginPlay()
{
	Super::BeginPlay();
	if(HasAuthority())
	{
		AddReplicatedSubObject(TotalTrees);
		AddReplicatedSubObject(TotalForage);
		AddReplicatedSubObject(TotalWildlife);
		AddReplicatedSubObject(CombatSystem);
		AddReplicatedSubObject(StartParameter);
	}
	// Spawn Static Mesh Batcher
	StaticMeshBatcher = GetWorld()->SpawnActor<AStaticMeshBatcher>();
}

void AGS_Ingame::EndGame_Implementation(::EGameEnding Ending, const FString& EndingMessage)
{
	if (GameEnded) return;
	GameEnded = true;
	OnGameEnding.Broadcast(Ending, EndingMessage);
}

void AGS_Ingame::CountIslandMaxEcoValues()
{
	TileMap->CountAllMaxEcoValues(IslandMaxTrees, IslandMaxWildlife, IslandMaxForage);
}
