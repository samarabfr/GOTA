// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "DaytimeManager.generated.h"

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

	// --------------- Current Time ---------------


private:
	UPROPERTY(EditAnywhere, Category="Daytime")
	float CurrentTime;

public:
	float GetTime() { return CurrentTime; }
	void SetTime(const float NewTime);

	float GetFullDayLength() { return DayLength + NightLength; }

	void StartDay();
	void StartNight();
	
	// --------------- Settings ---------------
	
private:
	UPROPERTY(VisibleAnywhere, Category="Daytime")
	bool bIsDay;

	UPROPERTY(EditAnywhere, Category="Daytime")
	int32 DayLength = 10;

	UPROPERTY(EditAnywhere, Category="Daytime")
	int32 NightLength = 5;

	UPROPERTY(EditAnywhere, Category="Daytime")
	float DayTimeSpeed = 1.0f;

	UPROPERTY(EditAnywhere, Category="Daytime")
	float SunHeight;


	UPROPERTY(EditAnywhere, Category="DaytimeManager Setup")
	ADirectionalLight* SunActor;

	UPROPERTY(EditAnywhere, Category="DayTime Graphics")
	FLinearColor SunColor;

	UPROPERTY(EditAnywhere, Category="DayTime Graphics")
	float SunBrightness;

	UPROPERTY(EditAnywhere, Category="DayTime Graphics")
	FVector2D SunPitchRange;
	

	UPROPERTY(EditAnywhere, Category="DaytimeManager Setup")
	ADirectionalLight* MoonActor;

	UPROPERTY(EditAnywhere, Category="DayTime Graphics")
	FLinearColor MoonColor;

	UPROPERTY(EditAnywhere, Category="DayTime Graphics")
	float MoonBrightness;
	

	UPROPERTY(EditAnywhere, Category="DaytimeManager")
	float CloudSpeed = 1.0f;

	UPROPERTY(EditAnywhere, Category="DaytimeManager")
	float CloudOpacity = 1.0f;

	UPROPERTY(EditAnywhere, Category="DaytimeManager")
	float StarBrightness = 1.0f;

	UPROPERTY()
	UMaterialParameterCollectionInstance* ParameterCollection;

	UPROPERTY()
	UStaticMeshComponent* SkyboxComponent;

	UPROPERTY(EditAnywhere, Category="DaytimeManager Setup")
	UStaticMesh* SkyboxMesh;

	UPROPERTY(EditAnywhere, Category="DaytimeManager Setup")
	UMaterial* SkyboxMaterial;

	UPROPERTY()
	UMaterialInstanceDynamic* DynamicMaterial;


	UPROPERTY(EditAnywhere, Category="DaytimeManager Setup")
	UCurveLinearColor* HorizonColorCurve;

	UPROPERTY(EditAnywhere, Category="DaytimeManager Setup")
	UCurveLinearColor* ZenithColorCurve;

	UPROPERTY(EditAnywhere, Category="DaytimeManager Setup")
	UCurveLinearColor* CloudsColorCurve;

	UPROPERTY(EditAnywhere, Category="DaytimeManager Setup")
	FRuntimeFloatCurve HorizonFalloffCurve;



	void RefreshMaterial();
};
