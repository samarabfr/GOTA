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
	
	if (!SunActor || !MoonActor || !SkyLight || !HorizonFog || !SkyboxMaterial)
	{
		UE_LOG(LogTemp, Error, TEXT("DaytimeManager: One or more required actors are not set."));
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

		// Calculate sun's position on a tilted circular orbit
		const float DayProgressNormalized = FMath::Clamp(CurrentTime / DayLength, 0.0f, 1.0f);

		const float OrbitAngle = DayProgressNormalized * PI;

		const float X = FMath::Cos(OrbitAngle);
		const float Y = FMath::Sin(OrbitAngle) * FMath::Cos(FMath::DegreesToRadians(SunOrbitTilt));
		const float Z = FMath::Sin(OrbitAngle) * FMath::Sin(FMath::DegreesToRadians(SunOrbitTilt));

		// Convert to rotation angles
		const float NewSunElevation = FMath::RadiansToDegrees(FMath::Atan2(Z, FMath::Sqrt(X * X + Y * Y)));
		const float NewSunAzimuth = FMath::RadiansToDegrees(FMath::Atan2(Y, X));

		// Apply rotation
		SunActor->SetActorRotation(FRotator(-NewSunElevation, NewSunAzimuth, 0.0f));
		MoonActor->SetActorRotation(FRotator(-NewSunElevation + 180.0f, NewSunAzimuth, 0.0f));
	}
	else
	{
		// It should be Night
		if (bIsDay) StartNight();

		// Calculate sun's position on a tilted circular orbit
		const float NightProgressNormalized = FMath::Clamp((CurrentTime - DayLength) / NightLength, 0.0f, 1.0f);

		const float OrbitAngle = NightProgressNormalized * PI;

		const float X = FMath::Cos(OrbitAngle);
		const float Y = FMath::Sin(OrbitAngle) * FMath::Cos(FMath::DegreesToRadians(MoonOrbitTilt));
		const float Z = FMath::Sin(OrbitAngle) * FMath::Sin(FMath::DegreesToRadians(MoonOrbitTilt));

		// Convert to rotation angles
		const float NewSunElevation = FMath::RadiansToDegrees(FMath::Atan2(Z, FMath::Sqrt(X * X + Y * Y)));
		const float NewSunAzimuth = FMath::RadiansToDegrees(FMath::Atan2(Y, X));

		// Apply rotation
		SunActor->SetActorRotation(FRotator(-NewSunElevation + 180.0f, NewSunAzimuth, 0.0f));
		MoonActor->SetActorRotation(FRotator(-NewSunElevation, NewSunAzimuth, 0.0f));
	}

	if (ParameterCollection)
	{
		const float SmoothNight = FMath::GetMappedRangeValueClamped(
			FVector2D(-0.2f, 0.2f),
			FVector2D(1.0f, 0.0f),
			SunHeight);

		ParameterCollection->SetScalarParameterValue(FName("IsNightSmooth"), SmoothNight);
	}
}

float ADaytimeManager::GetDaytimeNormalized() const
{
	if (GetTime() <= GetDayLength())
	{
		float HalfDayLength = DayLength / 2.0f;

		if (CurrentTime <= HalfDayLength)
		{
			// Linearly map [0, HalfDayLength] to [0, 1]
			return CurrentTime / HalfDayLength;
		}
		// Linearly map [HalfDayLength, DayLength] to [1, 0]
		return 1.0f - ((CurrentTime - HalfDayLength) / HalfDayLength);
	}
	// It's nighttime
	float HalfNightLength = NightLength / 2.0f;
	float NightProgress = CurrentTime - DayLength;

	if (NightProgress <= HalfNightLength)
	{
		// Linearly map [0, HalfNightLength] to [0, -1]
		return -1.0f * NightProgress / HalfNightLength;
	}
	// Linearly map [HalfNightLength, NightLength] to [-1, 0]
	return - 1.0f + ((NightProgress - HalfNightLength) / HalfNightLength);
}

void ADaytimeManager::StartDay()
{
	bIsDay = true;
	MARK_PROPERTY_DIRTY_FROM_NAME(ADaytimeManager, CurrentTime, this)

	SunActor->SetCastShadows(true);
	MoonActor->SetCastShadows(false);
	
	if (ParameterCollection)
		ParameterCollection->SetScalarParameterValue(FName("IsNight"), 0.0f);
}

void ADaytimeManager::StartNight()
{
	bIsDay = false;
	MARK_PROPERTY_DIRTY_FROM_NAME(ADaytimeManager, CurrentTime, this)

	SunActor->SetCastShadows(false);
	MoonActor->SetCastShadows(true);
	
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
	                                         HorizonFalloffCurve->FloatCurve.Eval(SunHeight));

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
	RefreshLightIntensity();
	RefreshLightColors();
}

void ADaytimeManager::RefreshLightIntensity()
{
	// Sun
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
	// Skylight
	SkyLightComponent->SetIntensity(LightsIntensity.G);
	// moon
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

void ADaytimeManager::RefreshLightColors()
{
	const float Daytime = GetDaytimeNormalized();
	// Sun
	const FLinearColor SunLightColor = SunLightColorCurve->GetLinearColorValue(Daytime);
	if (SunLightComponent->IsVisible())
	{
		SunLightComponent->SetLightColor(SunLightColor);
	}
	// Skylight
	const FLinearColor SkylightLightColor = SkylightColorCurve->GetLinearColorValue(Daytime);
	if (SkyLightComponent->IsVisible())
	{
		SkyLightComponent->SetLightColor(SkylightLightColor);
	}
	// Skylight
	const FLinearColor MoonLightColor = MoonLightColorCurve->GetLinearColorValue(Daytime);
	if (MoonLightComponent->IsVisible())
	{
		MoonLightComponent->SetLightColor(MoonLightColor);
	}
}
