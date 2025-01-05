// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "Components/StateTreeComponent.h"
#include "StateTreeComponentArmy.generated.h"

UCLASS()
class GOTA_API UStateTreeComponentArmy : public UStateTreeComponent
{
	GENERATED_BODY()

	virtual void BeginPlay() override;
};
