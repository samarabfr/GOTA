// Fill out your copyright notice in the Description page of Project Settings.

#include "AbilityIndicator.h"

#include "NiagaraComponent.h"
#include "NiagaraSystem.h"

AAbilityIndicator::AAbilityIndicator()
{
	bReplicates = true;
	bAlwaysRelevant = true;
	bReplicateUsingRegisteredSubObjectList = false;
	SetNetUpdateFrequency(10.0f);

	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;
	PrimaryActorTick.TickInterval = 0.016f;

	RootComponent = CreateDefaultSubobject<USceneComponent>("ROOT");

	Rotator = CreateDefaultSubobject<USceneComponent>("Rotator");
	Rotator->SetupAttachment(RootComponent);

	StonePlateMesh = CreateDefaultSubobject<UStaticMeshComponent>("StonePlate");
	StonePlateMesh->SetupAttachment(Rotator);

	FrontIcon = CreateDefaultSubobject<UStaticMeshComponent>("FrontIcon");
	FrontIcon->SetupAttachment(Rotator);

	BackIcon = CreateDefaultSubobject<UStaticMeshComponent>("BackIcon");
	BackIcon->SetupAttachment(Rotator);
}

void AAbilityIndicator::BeginPlay()
{
	Super::BeginPlay();

	if (IconMaterial)
	{
		UMaterialInstanceDynamic* DynamicMat = UMaterialInstanceDynamic::Create(IconMaterial, this);
		FrontIcon->SetMaterial(0, IconMaterial);
		BackIcon->SetMaterial(0, IconMaterial);
	}

	FX_IndicatorComponent = NewObject<UNiagaraComponent>(this);
	FX_IndicatorComponent->SetAsset(NiagaraSystem);
	FX_IndicatorComponent->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
	FX_IndicatorComponent->RegisterComponent();
	FX_IndicatorComponent->Deactivate();
}

void AAbilityIndicator::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	Rotator->AddLocalRotation(FRotator(0, RotationSpeed * DeltaSeconds, 0));
}

void AAbilityIndicator::Activate()
{
	if (FX_IndicatorComponent)
	{
		FX_IndicatorComponent->SetFloatParameter(FName("AgeSpeed"), 1.0f);
		FX_IndicatorComponent->ActivateSystem();
	}
}

void AAbilityIndicator::Deactivate()
{
	if (FX_IndicatorComponent)
	{
		FX_IndicatorComponent->SetFloatParameter(FName("AgeSpeed"), 5.0f);
		FX_IndicatorComponent->Deactivate();
	}
}
