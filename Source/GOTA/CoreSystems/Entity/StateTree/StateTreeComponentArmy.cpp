// Fill out your copyright notice in the Description page of Project Settings.


#include "StateTreeComponentArmy.h"

#include "StateTree.h"

void UStateTreeComponentArmy::BeginPlay()
{
	Super::BeginPlay();
	// Define the asset path
	FSoftObjectPath StateTreePath(TEXT("/Game/CoreSystems/Entity/ST_Army.ST_Army"));
	// Load the asset synchronously
	UStateTree* LoadedStateTree = Cast<UStateTree>(StateTreePath.TryLoad());
	if (LoadedStateTree)
	{
		StateTreeRef.SetStateTree(LoadedStateTree);
	}
}
