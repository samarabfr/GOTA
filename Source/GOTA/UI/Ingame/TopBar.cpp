#include "TopBar.h"

#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "GOTA/CoreSystems/Faction/Settlement/Colony.h"
#include "GOTA/CoreSystems/Faction/Settlement/Tribe.h"
#include "GOTA/CoreSystems/GameplayFramework/DaytimeManager.h"
#include "GOTA/CoreSystems/GameplayFramework/GS_Ingame.h"


void UTopBar::NativeConstruct()
{
	Super::NativeConstruct();
	GameState = GetWorld()->GetGameState<AGS_Ingame>();
}

void UTopBar::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	RefreshClock();

	if (!GameState) return;
	int16 Colonists = GameState->GetColony()->Population->GetSize();
	int16 Natives = GameState->GetTribe()->Population->GetSize();
	if (Colonists + Natives != 0)
	{
		float NativeProportion = Natives / static_cast<float>(Colonists + Natives);
		float Angle = FMath::Lerp(-40.0f, 40.0f, 1.0f - NativeProportion);
		Power_Disk->SetRenderTransformAngle(Angle);
	}

	FGameResources ColonyRes = GameState->GetColony()->GetResources();
	Colony_Pop->SetText(FText::AsNumber(Colonists));
	Colony_Food->SetText(FText::AsNumber(static_cast<int32>(ColonyRes.Food)));
	Colony_Wood->SetText(FText::AsNumber(static_cast<int32>(ColonyRes.Wood)));
	Colony_Stone->SetText(FText::AsNumber(static_cast<int32>(ColonyRes.Stone)));

	FGameResources TribeRes = GameState->GetTribe()->GetResources();
	Tribe_Pop->SetText(FText::AsNumber(Natives));
	Tribe_Food->SetText(FText::AsNumber(static_cast<int32>(TribeRes.Food)));
	Tribe_Wood->SetText(FText::AsNumber(static_cast<int32>(TribeRes.Wood)));
	Tribe_Stone->SetText(FText::AsNumber(static_cast<int32>(TribeRes.Stone)));
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
