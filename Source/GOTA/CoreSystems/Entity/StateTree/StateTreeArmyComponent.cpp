// Fill out your copyright notice in the Description page of Project Settings.


#include "StateTreeArmyComponent.h"

#include "StateTree.h"

UStateTreeArmyComponent::UStateTreeArmyComponent()
{
	ConstructorHelpers::FObjectFinder<UStateTree> StateTreeFinder(
		TEXT("/Game/CoreSystems/Entity/ST_Army"));
	StateTreeRef.SetStateTree(StateTreeFinder.Object);
}
