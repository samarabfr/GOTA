// Fill out your copyright notice in the Description page of Project Settings.

#include "AbilityIndicator.h"

#include "Ability.h"
#include "AbilitySettings.h"
#include "Guardian.h"
#include "GuardianSettings.h"
#include "NiagaraComponent.h"
#include "NiagaraSystem.h"
#include "Components/WidgetComponent.h"
#include "GOTA/UI/Widgets/GotaImage.h"

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

	StonePlateMesh = CreateDefaultSubobject<UStaticMeshComponent>("Stone Plate");
	StonePlateMesh->SetupAttachment(Rotator);

	FrontIcon = CreateDefaultSubobject<UWidgetComponent>("Front Icon");
	FrontIcon->SetupAttachment(Rotator);

	BackIcon = CreateDefaultSubobject<UWidgetComponent>("Back Icon");
	BackIcon->SetupAttachment(Rotator);
	
	GuardianNiagaraEffect = CreateDefaultSubobject<UNiagaraComponent>("Guardian Effect");
	GuardianNiagaraEffect->SetupAttachment(RootComponent);
}

void AAbilityIndicator::BeginPlay()
{
	Super::BeginPlay();
	IconWidget = Cast<UGotaImage>(FrontIcon->GetWidget());
	if (IconWidget)
	{
		BackIcon->SetWidget(IconWidget);
	}
	StonePlateMesh->SetVisibility(false);
	FrontIcon->SetVisibility(false);
	BackIcon->SetVisibility(false);
}

void AAbilityIndicator::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	Rotator->AddLocalRotation(FRotator(0, RotationSpeed * DeltaSeconds, 0));
}

void AAbilityIndicator::SetGuardian(AGuardian* Guardian)
{
	if (!Guardian) return;

	UNiagaraSystem* GuardianSystem = Guardian->GetSettings()->GetAbilityIndicatorEffect();
	UNiagaraSystem* CurrentSystem = GuardianNiagaraEffect->GetAsset();
	if (GuardianSystem && GuardianSystem != CurrentSystem)
	{
		GuardianNiagaraEffect->SetAsset(GuardianSystem);
	}
}

void AAbilityIndicator::SetAbility(AAbility* Ability)
{
	if (!Ability) return;
	UTexture2D* Icon = Ability->GetSettings()->GetIcon();
	if (Icon && IconWidget)
	{
		IconWidget->SetImage(Icon);
	}
}

void AAbilityIndicator::SetTarget(FAbilityTarget NewAbilityTarget)
{
}

void AAbilityIndicator::Activate()
{
	if (GuardianNiagaraEffect)
	{
		GuardianNiagaraEffect->SetFloatParameter(FName("AgeSpeed"), 1.0f);
		GuardianNiagaraEffect->ActivateSystem();
	}
	StonePlateMesh->SetVisibility(true);
	FrontIcon->SetVisibility(true);
	BackIcon->SetVisibility(true);
}

void AAbilityIndicator::Deactivate()
{
	if (GuardianNiagaraEffect)
	{
		GuardianNiagaraEffect->SetFloatParameter(FName("AgeSpeed"), 5.0f);
		GuardianNiagaraEffect->Deactivate();
	}
	StonePlateMesh->SetVisibility(false);
	FrontIcon->SetVisibility(false);
	BackIcon->SetVisibility(false);
}
