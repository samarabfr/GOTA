#include "TopBar.h"

#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "GOTA/Settlement/SettlementPopulation.h"
#include "GOTA/Utility/DaytimeManager.h"
#include "GOTA/GameplayFramework/GS_Ingame.h"
#include "GOTA/Settlement/Settlement.h"


void UTopBar::NativeConstruct()
{
	Super::NativeConstruct();
	GameState = GetWorld()->GetGameState<AGS_Ingame>();
}

void UTopBar::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	RefreshClock();

	ResourceUpdateTimeCounter += InDeltaTime;
	if (ResourceUpdateTimeCounter < SecondsBeforeResourceUpdate)
		return;
	ResourceUpdateTimeCounter = 0.0f;

	if (!GameState || !GameState->GetColony() || !GameState->GetTribe()) return;
	int16 Colonists = GameState->GetColony()->GetPopulation()->GetSize();
	int16 Natives = GameState->GetTribe()->GetPopulation()->GetSize();
	if (Colonists + Natives != 0)
	{
		float NativeProportion = Natives / static_cast<float>(Colonists + Natives);
		float Angle = FMath::Lerp(-40.0f, 40.0f, 1.0f - NativeProportion);
		Power_Disk->SetRenderTransformAngle(Angle);
	}

	FConstructionResources ColonyRes = GameState->GetColony()->GetResources();
	Colony_Pop->SetText(FText::AsNumber(Colonists));
	Colony_Food->SetText(FText::AsNumber(static_cast<int32>(ColonyRes.Food)));
	Colony_Wood->SetText(FText::AsNumber(static_cast<int32>(ColonyRes.Wood)));
	Colony_Stone->SetText(FText::AsNumber(static_cast<int32>(ColonyRes.Stone)));

	FConstructionResources ColonyResIncome = GameState->GetColony()->GetEffectiveProduction();
	UpdateIncomeNumber(TXT_ColonyFoodIncome, ColonyResIncome.Food);
	UpdateIncomeNumber(TXT_ColonyWoodIncome, ColonyResIncome.Wood);
	UpdateIncomeNumber(TXT_ColonyStoneIncome, ColonyResIncome.Stone);

	FConstructionResources TribeRes = GameState->GetTribe()->GetResources();
	Tribe_Pop->SetText(FText::AsNumber(Natives));
	Tribe_Food->SetText(FText::AsNumber(static_cast<int32>(TribeRes.Food)));
	Tribe_Wood->SetText(FText::AsNumber(static_cast<int32>(TribeRes.Wood)));
	Tribe_Stone->SetText(FText::AsNumber(static_cast<int32>(TribeRes.Stone)));

	FConstructionResources TribeResIncome = GameState->GetTribe()->GetEffectiveProduction();
	UpdateIncomeNumber(TXT_TribeFoodIncome, TribeResIncome.Food);
	UpdateIncomeNumber(TXT_TribeWoodIncome, TribeResIncome.Wood);
	UpdateIncomeNumber(TXT_TribeStoneIncome, TribeResIncome.Stone);
}

void UTopBar::RefreshClock()
{
	if (!GameState || !GameState->DaytimeManager) return;
	const float SunHeight = GameState->DaytimeManager->GetDaytimeNormalized();

	float NewRotation = 0.0f;
	if (SunHeight < 0)
	{
		// Night
		NewRotation = FMath::GetMappedRangeValueUnclamped(
			FVector2D(0, -1),
			FVector2D(0, MaxNightRotation),
			SunHeight);
	}
	else
	{
		// Day
		NewRotation = FMath::GetMappedRangeValueUnclamped(
			FVector2D(0, 1),
			FVector2D(0, MaxDayRotation),
			SunHeight);
	}

	Daytime_Disk->SetRenderTransformAngle(NewRotation);
}

void UTopBar::UpdateIncomeNumber(UTextBlock* TextBlock, const float IncomeAmount)
{
	const int32 IncomePerMinuteFloored = static_cast<int32>(IncomeAmount * 60);
	if (IncomePerMinuteFloored == 0)
	{
		TextBlock->SetVisibility(ESlateVisibility::Hidden);
		return;
	}
	if (IncomePerMinuteFloored > 0)
	{
		TextBlock->SetColorAndOpacity(FSlateColor(FLinearColor::Green));
	}
	else
	{
		TextBlock->SetColorAndOpacity(FSlateColor(FLinearColor::Red));
	}
	FNumberFormattingOptions FormattingOptions;
	FormattingOptions.AlwaysSign = true;
	TextBlock->SetText(FText::AsNumber(IncomePerMinuteFloored, &FormattingOptions));
	TextBlock->SetVisibility(ESlateVisibility::Visible);
}
