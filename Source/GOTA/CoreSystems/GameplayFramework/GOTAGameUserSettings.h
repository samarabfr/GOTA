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
	
	// ------------------- ShadowSettings -------------------
private:
	UPROPERTY(Config)
	bool bCascadedShadowMapsEnabled;

	UPROPERTY(Config)
	bool bDistanceFieldShadowsEnabled;

public:
	bool GetCascadedShadowMapsEnabled() const { return bCascadedShadowMapsEnabled; }
	void SetCascadedShadowMapsEnabled(bool NewEnabled) { bCascadedShadowMapsEnabled = NewEnabled; }

	bool GetDistanceFieldShadowsEnabled() const { return bDistanceFieldShadowsEnabled; }
	void SetDistanceFieldShadowsEnabled(bool NewEnabled) { bDistanceFieldShadowsEnabled = NewEnabled; }
};
