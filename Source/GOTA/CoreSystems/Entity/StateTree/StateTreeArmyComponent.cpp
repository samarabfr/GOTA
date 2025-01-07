// Fill out your copyright notice in the Description page of Project Settings.


#include "StateTreeArmyComponent.h"

#include "StateTree.h"

void UStateTreeArmyComponent::BeginPlay()
{
	Super::BeginPlay();
	const FSoftObjectPath StateTreePath(TEXT("/Game/CoreSystems/Entity/ST_Army.ST_Army"));
	UStateTree* LoadedStateTree = Cast<UStateTree>(StateTreePath.TryLoad());
	if (LoadedStateTree) StateTreeRef.SetStateTree(LoadedStateTree);
}
