// Fill out your copyright notice in the Description page of Project Settings.


#include "StateTreeCivilianComponent.h"

#include "StateTree.h"

UStateTreeCivilianComponent::UStateTreeCivilianComponent()
{
	ConstructorHelpers::FObjectFinder<UStateTree> StateTreeFinder(
		TEXT("/Game/CoreSystems/Entity/ST_Civilian"));
	StateTreeRef.SetStateTree(StateTreeFinder.Object);
}
