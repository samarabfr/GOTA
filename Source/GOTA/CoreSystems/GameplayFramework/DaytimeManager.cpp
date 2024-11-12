#include "DaytimeManager.h"

#include "GS_Ingame.h"
#include "Components/ExponentialHeightFogComponent.h"
#include "Components/LightComponent.h"
#include "Components/SkyLightComponent.h"
#include "Curves/CurveLinearColor.h"
#include "Engine/DirectionalLight.h"
#include "Engine/ExponentialHeightFog.h"
#include "Engine/SkyLight.h"
#include "Materials/MaterialParameterCollection.h"
#include "Materials/MaterialParameterCollectionInstance.h"
#include "Net/UnrealNetwork.h"
#include "Net/Core/PushModel/PushModel.h"

void ADaytimeManager::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	FDoRepLifetimeParams Params;
	Params.bIsPushBased = true;
	Params.Condition = COND_None;
	Params.RepNotifyCondition = REPNOTIFY_OnChanged;
	DOREPLIFETIME_WITH_PARAMS(ADaytimeManager, CurrentTime, Params)
}

ADaytimeManager::ADaytimeManager()
{
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = false;
	PrimaryActorTick.TickInterval = 0.0f;

	SkyboxComponent = CreateDefaultSubobject<UStaticMeshComponent>("Skybox");
	RootComponent = SkyboxComponent;
}

void ADaytimeManager::BeginPlay()
{
	Super::BeginPlay();
	GetWorld()->GetGameState<AGS_Ingame>()->DaytimeManager = this;

	const UMaterialParameterCollection* ParameterCollectionFinder = LoadObject<UMaterialParameterCollection>(
		nullptr, TEXT("/Game/Visuals/Materials/MPC_GlobalParams"));
	if (ParameterCollectionFinder)
	{
		ParameterCollection = GetWorld()->GetParameterCollectionInstance(ParameterCollectionFinder);
	}
	
	if (!SkyboxMaterial)
	{
		UE_LOG(LogTemp, Warning, TEXT("DaytimeManager missing Material"))
		return;
	}

	DynamicMaterial = SkyboxComponent->CreateDynamicMaterialInstance(0, SkyboxMaterial);

	SunLightComponent = SunActor->GetLightComponent();
	SkyLightComponent = SkyLight->GetLightComponent();
	MoonLightComponent = MoonActor->GetLightComponent();

	SetTime(DayLength * 0.3f);
}


void ADaytimeManager::Tick(const float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	SetTime(GetTime() + (DayTimeSpeed * DeltaSeconds) / 60);
	RefreshSunHeight();
	RefreshMaterial();
	RefreshLightSetup();
}

void ADaytimeManager::SetTime(const float NewTime)
{
	CurrentTime = NewTime < 0 ? 0 : FMath::Fmod(NewTime, GetFullDayLength());
	if (CurrentTime <= DayLength)
	{
		// It should be Day
		if (!bIsDay) StartDay();

		const float NewSunPitch = FMath::GetMappedRangeValueUnclamped(
			FVector2D(0, DayLength),
			FVector2D(-180, 0),
			CurrentTime);

		SunActor->SetActorRotation(FRotator(NewSunPitch, 0, 0));
		MoonActor->SetActorRotation(FRotator(NewSunPitch + 180.0, 0, 0));
	}
	else
	{
		// It should be Night
		if (bIsDay) StartNight();

		const float NewMoonPitch = FMath::GetMappedRangeValueUnclamped(
			FVector2D(0, NightLength),
			FVector2D(-180, 0),
			CurrentTime);

		SunActor->SetActorRotation(FRotator(NewMoonPitch + 180.0, 0, 0));
		MoonActor->SetActorRotation(FRotator(NewMoonPitch, 0, 0));
	}

	if (ParameterCollection)
	{
		float SmoothNight;
		if (SunHeight < -0.2f)
		{
			SmoothNight = 1.0f;
		}
		else if (SunHeight > 0.2f)
		{
			SmoothNight = 0.0f;
		}
		else
		{
			SmoothNight = FMath::GetMappedRangeValueUnclamped(
				FVector2D(-0.2f, 0.2f),
				FVector2D(1.0f, 0.0f),
				SunHeight);
		}
		ParameterCollection->SetScalarParameterValue(FName("IsNightSmooth"), SmoothNight);
	}
}

void ADaytimeManager::StartDay()
{
	bIsDay = true;
	MARK_PROPERTY_DIRTY_FROM_NAME(ADaytimeManager, CurrentTime, this)

	if (ParameterCollection)
		ParameterCollection->SetScalarParameterValue(FName("IsNight"), 0.0f);
}

void ADaytimeManager::StartNight()
{
	bIsDay = false;
	MARK_PROPERTY_DIRTY_FROM_NAME(ADaytimeManager, CurrentTime, this)

	if (ParameterCollection)
		ParameterCollection->SetScalarParameterValue(FName("IsNight"), 1.0f);
}

void ADaytimeManager::RefreshSunHeight()
{
	SunHeight = FMath::GetMappedRangeValueUnclamped(
		FVector2D(0, -90),
		FVector2D(0, 1),
		SunActor->GetActorRotation().Pitch);
}

void ADaytimeManager::RefreshMaterial()
{
	if (!DynamicMaterial) return;
	DynamicMaterial->SetVectorParameterValue(FName("HorizonColor"),
	                                         HorizonColorCurve->GetClampedLinearColorValue(SunHeight));
	DynamicMaterial->SetVectorParameterValue(FName("ZenithColor"),
	                                         ZenithColorCurve->GetClampedLinearColorValue(SunHeight));
	DynamicMaterial->SetVectorParameterValue(FName("CloudsColor"),
	                                         CloudsColorCurve->GetClampedLinearColorValue(SunHeight));

	DynamicMaterial->SetScalarParameterValue(FName("HorizonFalloff"),
	                                         HorizonFalloffCurve.GetRichCurveConst()->Eval(SunHeight));

	const FRotator SunRotator = SunActor->GetActorRotation();
	DynamicMaterial->SetVectorParameterValue(FName("SunlightDirection"), SunRotator.Vector());
	DynamicMaterial->SetVectorParameterValue(FName("SunColor"), SunColor);
	DynamicMaterial->SetScalarParameterValue(FName("SunBrightness"), SunBrightness);

	const FRotator MoonRotator = MoonActor->GetActorRotation();
	DynamicMaterial->SetVectorParameterValue(FName("MoonlightDirection"), MoonRotator.Vector());
	DynamicMaterial->SetVectorParameterValue(FName("MoonColor"), MoonColor);
	DynamicMaterial->SetScalarParameterValue(FName("MoonBrightness"), MoonBrightness);

	DynamicMaterial->SetScalarParameterValue(FName("CloudSpeed"), CloudSpeed);
	DynamicMaterial->SetScalarParameterValue(FName("CloudOpacity"), CloudOpacity);

	const float StarOpacity = SunHeight < 0 ? FMath::Abs(SunHeight) : 0;
	DynamicMaterial->SetScalarParameterValue(FName("StarOpacity"), StarOpacity);
	DynamicMaterial->SetScalarParameterValue(FName("StarBrightness"), StarBrightness);
}

void ADaytimeManager::RefreshLightSetup()
{
	HorizonFog->GetComponent()->SetFogInscatteringColor(HorizonFogColorCurve->GetClampedLinearColorValue(SunHeight));

	const FLinearColor LightsIntensity = LightsIntensityCurve->GetLinearColorValue(SunHeight);
	if (LightsIntensity.R <= 0.0f)
	{
		if (SunLightComponent->IsVisible())
			SunLightComponent->SetVisibility(false);
	}
	else
	{
		if (!SunLightComponent->IsVisible())
			SunLightComponent->SetVisibility(true);
		SunLightComponent->SetIntensity(LightsIntensity.R);
	}

	SkyLightComponent->SetIntensity(LightsIntensity.G);

	if (LightsIntensity.B <= 0.0f)
	{
		if (MoonLightComponent->IsVisible())
			MoonLightComponent->SetVisibility(false);
	}
	else
	{
		if (!MoonLightComponent->IsVisible())
			MoonLightComponent->SetVisibility(true);
		MoonLightComponent->SetIntensity(LightsIntensity.B);
	}
}
