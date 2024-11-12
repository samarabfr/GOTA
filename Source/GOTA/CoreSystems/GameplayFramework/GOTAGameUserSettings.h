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
	UPROPERTY(Config)
	bool bCascadedShadowMapsEnabled;

	UPROPERTY(Config)
	bool bDistanceFieldShadowsEnabled;

public:
	bool IsCascadedShadowMapsEnabled() const { return bCascadedShadowMapsEnabled; }
	void SetCascadedShadowMapsEnabled(const bool NewEnabled) { bCascadedShadowMapsEnabled = NewEnabled; }

	bool IsDistanceFieldShadowsEnabled() const { return bDistanceFieldShadowsEnabled; }
	void SetDistanceFieldShadowsEnabled(const bool NewEnabled) { bDistanceFieldShadowsEnabled = NewEnabled; }

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
};
