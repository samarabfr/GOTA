#include "DebugMenuTimeControls.h"

#include "Components/Button.h"
#include "Components/EditableTextBox.h"
#include "Components/Slider.h"
#include "GOTA/Utility/DaytimeManager.h"
#include "GOTA/GameplayFramework/GS_Ingame.h"
#include "Kismet/GameplayStatics.h"

// -------------------------------------------- LifeCycle --------------------------------------------

void UDebugMenuTimeControls::NativeConstruct()
{
	Super::NativeConstruct();
	if (const AGS_Ingame* GameState = GetWorld()->GetGameState<AGS_Ingame>())
	{
		DaytimeManager = GameState->DaytimeManager;
	}
	if (DaytimeManager.IsValid())
	{
		Slider_Daytime->SetMaxValue(DaytimeManager->GetFullDayLength());
	}

	BTN_Morning->OnClicked.AddDynamic(this, &UDebugMenuTimeControls::SetTimeMorning);
	BTN_Midday->OnClicked.AddDynamic(this, &UDebugMenuTimeControls::SetTimeMidday);
	BTN_Evening->OnClicked.AddDynamic(this, &UDebugMenuTimeControls::SetTimeEvening);
	BTN_Night->OnClicked.AddDynamic(this, &UDebugMenuTimeControls::SetTimeNight);

	BTN_Skip->OnClicked.AddDynamic(this, &UDebugMenuTimeControls::SkipToNextTime);

	Slider_Daytime->OnValueChanged.AddDynamic(this, &UDebugMenuTimeControls::SetDaytime);
	Slider_Daytime->OnMouseCaptureBegin.AddDynamic(this, &UDebugMenuTimeControls::SetSliderMouseCapturedTrue);
	Slider_Daytime->OnMouseCaptureEnd.AddDynamic(this, &UDebugMenuTimeControls::SetSliderMouseCapturedFalse);

	BTN_Stop->OnClicked.AddDynamic(this, &UDebugMenuTimeControls::SetDaytimeSpeedZero);
	BTN_Normal->OnClicked.AddDynamic(this, &UDebugMenuTimeControls::SetDaytimeSpeedOne);
	BTN_Fast->OnClicked.AddDynamic(this, &UDebugMenuTimeControls::SetDaytimeSpeedTwenty);

	TB_Speed->OnTextCommitted.AddDynamic(this, &UDebugMenuTimeControls::SetDaytimeSpeedCustom);
	TB_Speed->OnTextChanged.AddDynamic(this, &UDebugMenuTimeControls::ValidateDaytimeSpeedTextBox);

	BTN_GameStop->OnClicked.AddDynamic(this, &UDebugMenuTimeControls::SetGameDilationZero);
	BTN_GameNormal->OnClicked.AddDynamic(this, &UDebugMenuTimeControls::SetGameDilationOne);
	BTN_GameFast->OnClicked.AddDynamic(this, &UDebugMenuTimeControls::SetGameDilationFive);

	TB_GameSpeed->OnTextCommitted.AddDynamic(this, &UDebugMenuTimeControls::SetGameDilationCustom);
	TB_GameSpeed->OnTextChanged.AddDynamic(this, &UDebugMenuTimeControls::ValidateGameSpeedTextBox);
}

void UDebugMenuTimeControls::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	if (DaytimeManager.IsValid())
	{
		if (!bIsSliderMouseCaptured)
		{
			Slider_Daytime->SetValue(DaytimeManager->GetTime());
		}
		if (!TB_Speed->HasKeyboardFocus())
		{
			TB_Speed->SetText(FText::AsNumber(DaytimeManager->GetDayTimeSpeed()));
		}
	}
}

// -------------------------------------------- Utility --------------------------------------------

void UDebugMenuTimeControls::ValidateFloatText(const FText& InText, FString& OutString)
{
	FString InputString = InText.ToString();
	bool FoundDot = false;

	for (int32 i = 0; i < InputString.Len(); i++)
	{
		const TCHAR Char = InputString[i];
		if (FChar::IsDigit(Char))
		{
			OutString.AppendChar(Char);
		}
		if ((Char == '.') && !FoundDot)
		{
			OutString.AppendChar(Char);
			FoundDot = true;
		}
	}
}

// -------------------------------------------- Daytime --------------------------------------------

void UDebugMenuTimeControls::SetTimeMorning()
{
	if (DaytimeManager.IsValid())
		DaytimeManager->SetTime(TimeMarkers[0]);
}

void UDebugMenuTimeControls::SetTimeMidday()
{
	if (DaytimeManager.IsValid())
		DaytimeManager->SetTime(TimeMarkers[1]);
}

void UDebugMenuTimeControls::SetTimeEvening()
{
	if (DaytimeManager.IsValid())
		DaytimeManager->SetTime(TimeMarkers[2]);
}

void UDebugMenuTimeControls::SetTimeNight()
{
	if (DaytimeManager.IsValid())
		DaytimeManager->SetTime(TimeMarkers[3]);
}

void UDebugMenuTimeControls::SkipToNextTime()
{
	if (!DaytimeManager.IsValid()) return;

	const float CurrentTime = DaytimeManager->GetTime();

	for (const float Time : TimeMarkers)
	{
		if (CurrentTime < Time)
		{
			DaytimeManager->SetTime(Time);
			return;
		}
	}
	DaytimeManager->SetTime(TimeMarkers[0]);
}

// -------------------------------------------- Daytime Slider --------------------------------------------

void UDebugMenuTimeControls::SetDaytime(const float InValue)
{
	if (DaytimeManager.IsValid())
		DaytimeManager->SetTime(InValue);
}

// -------------------------------------------- Daytime Speed --------------------------------------------

void UDebugMenuTimeControls::SetDaytimeSpeedZero()
{
	if (DaytimeManager.IsValid())
		DaytimeManager->SetDaytimeSpeed(0.0f);
}

void UDebugMenuTimeControls::SetDaytimeSpeedOne()
{
	if (DaytimeManager.IsValid())
		DaytimeManager->SetDaytimeSpeed(1.0f);
}

void UDebugMenuTimeControls::SetDaytimeSpeedTwenty()
{
	if (DaytimeManager.IsValid())
		DaytimeManager->SetDaytimeSpeed(20.0f);
}

void UDebugMenuTimeControls::SetDaytimeSpeedCustom(const FText& Text, const ETextCommit::Type CommitMethod)
{
	const float Speed = FCString::Atof(*Text.ToString());
	if (DaytimeManager.IsValid())
		DaytimeManager->SetDaytimeSpeed(Speed);
}

void UDebugMenuTimeControls::ValidateDaytimeSpeedTextBox(const FText& InText)
{
	FString OutString;
	ValidateFloatText(InText, OutString);
	TB_Speed->SetText(FText::FromString(OutString));
}

// -------------------------------------------- Game Speed --------------------------------------------

void UDebugMenuTimeControls::SetGameDilationZero()
{
	UGameplayStatics::SetGlobalTimeDilation(this, 0.0f);
}

void UDebugMenuTimeControls::SetGameDilationOne()
{
	UGameplayStatics::SetGlobalTimeDilation(this, 1.0f);
}

void UDebugMenuTimeControls::SetGameDilationFive()
{
	UGameplayStatics::SetGlobalTimeDilation(this, 5.0f);
}

void UDebugMenuTimeControls::SetGameDilationCustom(const FText& Text, const ETextCommit::Type CommitMethod)
{
	const float Speed = FCString::Atof(*Text.ToString());
	UGameplayStatics::SetGlobalTimeDilation(this, Speed);
}

void UDebugMenuTimeControls::ValidateGameSpeedTextBox(const FText& InText)
{
	FString OutString;
	ValidateFloatText(InText, OutString);
	TB_GameSpeed->SetText(FText::FromString(OutString));
}
