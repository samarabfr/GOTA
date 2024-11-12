#include "GOTAGameUserSettings.h"


// ------------------- LifeCycle -------------------

UGOTAGameUserSettings::UGOTAGameUserSettings()
{
	bCascadedShadowMapsEnabled = true;
	bDistanceFieldShadowsEnabled = true;
	AntiAliasingType = 4;
}


// ------------------- Utility -------------------

void UGOTAGameUserSettings::ApplySettings(bool bCheckForCommandLineOverrides)
{
	Super::ApplySettings(bCheckForCommandLineOverrides);

	IConsoleVariable* CVarCascadedShadows = IConsoleManager::Get().
		FindConsoleVariable(TEXT("r.Shadow.CSM.MaxCascades"));
	if (CVarCascadedShadows)
	{
		// 0*3 = O means disabled, 1*3 = 3 means enabled
		CVarCascadedShadows->Set(bCascadedShadowMapsEnabled * 3);
	}

	IConsoleVariable* CVarDFShadows = IConsoleManager::Get().
		FindConsoleVariable(TEXT("r.DFShadowQuality"));
	if (CVarDFShadows)
	{
		// 0*3 = O means disabled, 1*3 = 3 means enabled
		CVarDFShadows->Set(bDistanceFieldShadowsEnabled * 3);
	}

	IConsoleVariable* CVarAntiAliasingType = IConsoleManager::Get().
		FindConsoleVariable(TEXT("r.AntiAliasingMethod"));
	if (CVarAntiAliasingType)
	{
		CVarAntiAliasingType->Set(AntiAliasingType);
	}
}


// ------------------- Overall Scalability -------------------

int32 UGOTAGameUserSettings::GOTAGetOverallScalabilityLevel() const
{
	int32 Target = GetShadowQuality();
	if (Target == GetAntiAliasingQuality())
	{
		return Target;
	}
	return -1;
}

void UGOTAGameUserSettings::GOTASetOverallScalabilityLevel(int32 NewScalability)
{
	SetShadowQuality(NewScalability);
	SetAntiAliasingQuality(NewScalability);
}
