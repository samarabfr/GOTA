// Fill out your copyright notice in the Description page of Project Settings.


#include "StateTreeComponentArmy.h"

#include "StateTree.h"

UStateTreeComponentArmy::UStateTreeComponentArmy()
{
	ConstructorHelpers::FObjectFinder<UStateTree> StateTreeFinder(
		TEXT("/Game/CoreSystems/Entity/ST_Army"));
	StateTreeRef.SetStateTree(StateTreeFinder.Object);
}
