#include "TopBar.h"

#include "Blueprint/WidgetLayoutLibrary.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "GOTA/CoreSystems/GameplayFramework/GS_Ingame.h"


void UTopBar::NativeConstruct()
{
	Super::NativeConstruct();
	GameState = GetWorld()->GetGameState<AGS_Ingame>();
}

void UTopBar::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	float Gametime = GetWorld()->GetTimeSeconds();
	float RotationPerSecond = 1;
	float NewRotation = static_cast<int32>(Gametime * RotationPerSecond);
	Daytime_Disk->SetRenderTransformAngle(Gametime);
	
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
	Colony_Food->SetText(FText::AsNumber(ColonyRes.Food));
	Colony_Wood->SetText(FText::AsNumber(ColonyRes.Wood));
	Colony_Stone->SetText(FText::AsNumber(ColonyRes.Stone));

	FGameResources TribeRes = GameState->GetTribe()->GetResources();
	Tribe_Pop->SetText(FText::AsNumber(Natives));
	Tribe_Food->SetText(FText::AsNumber(TribeRes.Food));
	Tribe_Wood->SetText(FText::AsNumber(TribeRes.Wood));
	Tribe_Stone->SetText(FText::AsNumber(TribeRes.Stone));
}
