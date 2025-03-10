#pragma once

#include "GameFramework/GameUserSettings.h"
#include "GotaGameUserSettings.generated.h"

UCLASS()
class GOTA_API UGotaGameUserSettings : public UGameUserSettings
{
	GENERATED_BODY()

	// -------------------------------------- LifeCycle --------------------------------------

	UGotaGameUserSettings();

	// -------------------------------------- Utility --------------------------------------
public:
	virtual void ApplySettings(bool bCheckForCommandLineOverrides) override;

	// -------------------------------------- Shadows --------------------------------------
private:
	// Can be between 0.0f and 2.0f
	UPROPERTY(Config)
	float ShadowDistanceFactor;

	void ApplyShadowSettings();

public:
	float GetShadowDistanceFactor() const { return ShadowDistanceFactor; }

	void SetShadowDistanceFactor(const float NewShadowDistanceFactor)
	{
		ShadowDistanceFactor = NewShadowDistanceFactor;
	}

	// -------------------------------------- Anti Aliasing --------------------------------------
private:
	// 0 = Off, 1 = FXAA, 2 = TAA, 4 = TSR
	UPROPERTY(Config)
	int32 AntiAliasingType;

	void ApplyAntiAliasingSettings();

public:
	// 0 = Off, 1 = FXAA, 2 = TAA, 4 = TSR
	int32 GetAntiAliasingType() const { return AntiAliasingType; }

	// 0 = Off, 1 = FXAA, 2 = TAA, 4 = TSR
	void SetAntiAliasingType(const int32 NewType) { AntiAliasingType = NewType; }

	// -------------------------------------- FPS --------------------------------------
private:
	UPROPERTY(Config)
	bool bUsingFPSLimit;

	UPROPERTY(Config)
	int32 FPSLimit;

	void ApplyFPSSettings();

public:
	bool IsUsingFPSLimit() const { return bUsingFPSLimit; }
	void SetIsUsingFPSLimit(const bool NewUsingFPSLimit) { bUsingFPSLimit = NewUsingFPSLimit; }

	int32 GetFPSLimit() const { return FPSLimit; }
	void SetFPSLimit(const int32 NewFPSLimit) { FPSLimit = NewFPSLimit; }

	// -------------------------------------- Sharpen --------------------------------------
private:
	UPROPERTY(Config)
	int32 Sharpen;

	void ApplySharpenSettings();
	
public:
	int32 GetSharpen() const { return Sharpen; }
	void SetSharpen(const int32 NewSharpen) { Sharpen = NewSharpen; }
};
