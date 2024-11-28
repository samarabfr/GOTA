#include "GOTAGameUserSettings.h"

// ------------------- LifeCycle -------------------

UGOTAGameUserSettings::UGOTAGameUserSettings()
{
	ShadowDistanceFactor = 1.0f;
	AntiAliasingType = 4;
	bUsingFPSLimit = false;
	FPSLimit = 60;
}


// ------------------- Utility -------------------

void UGOTAGameUserSettings::ApplySettings(bool bCheckForCommandLineOverrides)
{
	Super::ApplySettings(bCheckForCommandLineOverrides);
	
	IConsoleVariable* CVar_LimitFPS = IConsoleManager::Get().FindConsoleVariable(TEXT("t.MaxFPS"));
	if (CVar_LimitFPS)
	{
		if (bUsingFPSLimit)
		{
			CVar_LimitFPS->Set(FPSLimit);
		}
		else
		{
			CVar_LimitFPS->Set(0.0f);
		}
	}

	IConsoleVariable* CVar_TAAHistoryScreenPercentage =
		IConsoleManager::Get().FindConsoleVariable(TEXT("r.TemporalAA.HistoryScreenpercentage"));
	if (CVar_TAAHistoryScreenPercentage)
	{
		switch (GetAntiAliasingQuality())
		{
		case 3: // High
			CVar_TAAHistoryScreenPercentage->Set(200.0f);
			break;

		case 2: //Medium
			CVar_TAAHistoryScreenPercentage->Set(150.0f);
			break;

		case 1: // Low
			CVar_TAAHistoryScreenPercentage->Set(100.0f);
			break;

		default:
			break;
		}
	}
	
	IConsoleVariable* CVar_AntiAliasingType = IConsoleManager::Get().
		FindConsoleVariable(TEXT("r.AntiAliasingMethod"));
	if (CVar_AntiAliasingType)
	{
		CVar_AntiAliasingType->Set(AntiAliasingType);
	}

	IConsoleVariable* CVar_ShadowDistanceScale = IConsoleManager::Get().
	FindConsoleVariable(TEXT("r.Shadow.DistanceScale"));
	if (CVar_ShadowDistanceScale)
	{
		CVar_ShadowDistanceScale->Set(ShadowDistanceFactor);
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
