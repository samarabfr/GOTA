// Fill out your copyright notice in the Description page of Project Settings.

#include "AbilityIndicator.h"

#include "NiagaraComponent.h"
#include "NiagaraSystem.h"

AAbilityIndicator::AAbilityIndicator()
{
	bReplicates = true;
	bAlwaysRelevant = true;
	bReplicateUsingRegisteredSubObjectList = false;
	SetNetUpdateFrequency(1.0f);

	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = false;
	PrimaryActorTick.TickInterval = 0.5f;

	RootComponent = CreateDefaultSubobject<USceneComponent>("ROOT");
}

void AAbilityIndicator::BeginPlay()
{
	Super::BeginPlay();

	static const TCHAR* PathToDataTable = TEXT("/Game/Visuals/VFX/FXS_AbilityIndicator");
	if (UNiagaraSystem* NiagaraSystem = LoadObject<UNiagaraSystem>(nullptr, PathToDataTable))
	{
		FX_IndicatorComponent = NewObject<UNiagaraComponent>(this);
		FX_IndicatorComponent->SetAsset(NiagaraSystem);
		FX_IndicatorComponent->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
		FX_IndicatorComponent->RegisterComponent();
		FX_IndicatorComponent->Deactivate();
	}
}

void AAbilityIndicator::Activate()
{
	if (FX_IndicatorComponent)
	{
		FX_IndicatorComponent->ResetSystem();
		FX_IndicatorComponent->ActivateSystem();
	}
}

void AAbilityIndicator::Deactivate()
{
	if (FX_IndicatorComponent)
	{
		FX_IndicatorComponent->Deactivate();
	}
}
