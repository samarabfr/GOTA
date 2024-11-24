#pragma once

#include "GameFramework/GameUserSettings.h"
#include "GOTAGameUserSettings.generated.h"

UCLASS()
class GOTA_API UGOTAGameUserSettings : public UGameUserSettings
{
	GENERATED_BODY()

	// ------------------- LifeCycle -------------------

	UGOTAGameUserSettings();

	// ------------------- Utility -------------------
public:
	virtual void ApplySettings(bool bCheckForCommandLineOverrides) override;

	// ------------------- Overall Scalability -------------------

	// Similar to GetOverallScalabilityLevel(), but only considers the Scalability Settings we actually use
	// and doesn't consider ResolutionScale
	int32 GOTAGetOverallScalabilityLevel() const;

	// Similar to SetOverallScalabilityLevel(), but only sets the Scalability Settings we actually use
	// and doesn't set ResolutionScale
	void GOTASetOverallScalabilityLevel(int32 NewScalability);

	// ------------------- Shadows -------------------
private:
	// Can be between 0.0f and 2.0f
	UPROPERTY(Config)
	float ShadowDistanceFactor;

public:
	float GetShadowDistanceFactor() const { return ShadowDistanceFactor; }
	void SetShadowDistanceFactor(const float NewShadowDistanceFactor) { ShadowDistanceFactor = NewShadowDistanceFactor; }

	// ------------------- Anti Aliasing -------------------
private:
	// 0 = Off, 1 = FXAA, 2 = TAA, 4 = TSR
	UPROPERTY(Config)
	int32 AntiAliasingType;

public:
	// 0 = Off, 1 = FXAA, 2 = TAA, 4 = TSR
	int32 GetAntiAliasingType() const { return AntiAliasingType; }

	// 0 = Off, 1 = FXAA, 2 = TAA, 4 = TSR
	void SetAntiAliasingType(const int32 NewType) { AntiAliasingType = NewType; }

	// ------------------- FPS -------------------
private:
	UPROPERTY(Config)
	bool bUsingFPSLimit;

	UPROPERTY(Config)
	int32 FPSLimit;

public:
	bool IsUsingFPSLimit() const { return bUsingFPSLimit; }
	void SetIsUsingFPSLimit(const bool NewUsingFPSLimit) { bUsingFPSLimit = NewUsingFPSLimit; }

	int32 GetFPSLimit() const { return FPSLimit; }
	void SetFPSLimit(const int32 NewFPSLimit) { FPSLimit = NewFPSLimit; }
};
