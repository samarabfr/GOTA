// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "DaytimeManager.generated.h"

class ASkyLight;
class AExponentialHeightFog;
class ADirectionalLight;

UCLASS()
class GOTA_API ADaytimeManager : public AActor
{
	GENERATED_BODY()
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	// --------------- LifeCycle ---------------

private:
	ADaytimeManager();

	virtual void BeginPlay() override;

	virtual void Tick(float DeltaSeconds) override;

	// --------------- Time ---------------


private:
	UPROPERTY(EditAnywhere, Category="Daytime Settings", Replicated)
	float CurrentTime;

public:
	float GetTime() { return CurrentTime; }
	void SetTime(const float NewTime);

	float GetDayLength() { return DayLength; }
	float GetNightLength() { return NightLength; }
	float GetFullDayLength() { return DayLength + NightLength; }

	// SunHeight is -1 on midnight, 1 on midday and 0 on Dawn/Dusk
	float GetSunHeight() { return SunHeight; }

private:
	void StartDay();
	void StartNight();


	void RefreshSunHeight();

	void RefreshMaterial();

	void RefreshLightSetup();

	// --------------- Settings ---------------

private:
	UPROPERTY(VisibleAnywhere, Category="Daytime Settings")
	bool bIsDay;

	UPROPERTY(EditAnywhere, Category="Daytime Settings")
	int32 DayLength = 10;

	UPROPERTY(EditAnywhere, Category="Daytime Settings")
	int32 NightLength = 5;

	UPROPERTY(EditAnywhere, Category="Daytime Settings")
	float DayTimeSpeed = 1.0f;

	UPROPERTY(EditAnywhere, Category="Daytime Settings")
	float SunHeight;

	UPROPERTY(EditAnywhere, Category="Daytime Settings")
	float SunOrbitTilt = 70.0f;

	UPROPERTY(EditAnywhere, Category="Daytime Settings")
	float MoonOrbitTilt = 50.0f;

	UPROPERTY(EditAnywhere, Category="Daytime Settings")
	FLinearColor SunColor;

	UPROPERTY(EditAnywhere, Category="Daytime Settings")
	float SunBrightness;

	UPROPERTY(EditAnywhere, Category="Daytime Settings")
	FLinearColor MoonColor;

	UPROPERTY(EditAnywhere, Category="Daytime Settings")
	float MoonBrightness;

	UPROPERTY(EditAnywhere, Category="Daytime Settings")
	float CloudSpeed = 1.0f;

	UPROPERTY(EditAnywhere, Category="Daytime Settings")
	float CloudOpacity = 1.0f;

	UPROPERTY(EditAnywhere, Category="Daytime Settings")
	float StarBrightness = 1.0f;

	// --------------- References ---------------

	UPROPERTY(EditAnywhere, Category="DaytimeManager Setup")
	ADirectionalLight* SunActor;

	UPROPERTY(EditAnywhere, Category="DaytimeManager Setup")
	ASkyLight* SkyLight;

	UPROPERTY(EditAnywhere, Category="DaytimeManager Setup")
	ADirectionalLight* MoonActor;

	UPROPERTY(EditAnywhere, Category="DaytimeManager Setup")
	AExponentialHeightFog* HorizonFog;

	UPROPERTY(EditAnywhere, Category="DaytimeManager Setup")
	UMaterial* SkyboxMaterial;

	UPROPERTY(EditAnywhere, Category="DaytimeManager Setup")
	UCurveLinearColor* LightsIntensityCurve;

	UPROPERTY(EditAnywhere, Category="DaytimeManager Setup")
	UCurveLinearColor* HorizonColorCurve;

	UPROPERTY(EditAnywhere, Category="DaytimeManager Setup")
	UCurveLinearColor* ZenithColorCurve;

	UPROPERTY(EditAnywhere, Category="DaytimeManager Setup")
	UCurveLinearColor* CloudsColorCurve;

	UPROPERTY(EditAnywhere, Category="DaytimeManager Setup")
	UCurveLinearColor* HorizonFogColorCurve;

	UPROPERTY(EditAnywhere, Category="DaytimeManager Setup")
	UCurveFloat* HorizonFalloffCurve;

	UPROPERTY()
	UMaterialInstanceDynamic* DynamicMaterial;

	UPROPERTY()
	UMaterialParameterCollectionInstance* ParameterCollection;

	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* SkyboxComponent;

	UPROPERTY()
	ULightComponent* SunLightComponent;

	UPROPERTY()
	USkyLightComponent* SkyLightComponent;

	UPROPERTY()
	ULightComponent* MoonLightComponent;
};
