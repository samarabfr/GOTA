// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "Components/StateTreeComponent.h"
#include "StateTreeCivilianComponent.generated.h"

UCLASS()
class GOTA_API UStateTreeCivilianComponent : public UStateTreeComponent
{
	GENERATED_BODY()

public:
	void SetStateTree(UStateTree* StateTree);
};
