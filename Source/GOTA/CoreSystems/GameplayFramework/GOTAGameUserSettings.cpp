#include "GOTAGameUserSettings.h"

// ------------------- LifeCycle -------------------

UGOTAGameUserSettings::UGOTAGameUserSettings()
{
	ShadowDistanceFactor = 1.0f;
	AntiAliasingType = 4;
	bUsingFPSLimit = false;
	FPSLimit = 60;
	Sharpen = 1;
}

// ------------------- Utility -------------------

void UGOTAGameUserSettings::ApplySettings(bool bCheckForCommandLineOverrides)
{
	Super::ApplySettings(bCheckForCommandLineOverrides);
	ApplyShadowSettings();
	ApplyAntiAliasingSettings();
	ApplyFPSSettings();
}

// -------------------------------------- Shadows --------------------------------------

void UGOTAGameUserSettings::ApplyShadowSettings()
{
	IConsoleVariable* CVar_ShadowDistanceScale = IConsoleManager::Get().
		FindConsoleVariable(TEXT("r.Shadow.DistanceScale"));
	if (CVar_ShadowDistanceScale)
	{
		CVar_ShadowDistanceScale->Set(ShadowDistanceFactor);
	}
}

// -------------------------------------- Anti Aliasing --------------------------------------

void UGOTAGameUserSettings::ApplyAntiAliasingSettings()
{
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
}

// -------------------------------------- FPS --------------------------------------

void UGOTAGameUserSettings::ApplyFPSSettings()
{
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
}

