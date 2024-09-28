#include "TopBar.h"

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
	Daytime_Disk->SetRenderTransformAngle(Daytime_Disk->GetRenderTransformAngle() + 0.1);
	if (!GameState) return;
	int16 Colonists = GameState->Colony->PopulationSummary->GetSize();
	int16 Natives = GameState->Tribe->PopulationSummary->GetSize();
	if (Colonists + Natives == 0) return;
	float NativeProportion = Natives / static_cast<float>(Colonists + Natives);
	float Angle = FMath::Lerp(-40.0f, 40.0f, 1.0f - NativeProportion);
	Power_Disk->SetRenderTransformAngle(Angle);

	FGameResources ColonyRes = GameState->Colony->GetResources();
	Colony_Pop->SetText(FText::AsNumber(Colonists));
	Colony_Food->SetText(FText::AsNumber(ColonyRes.Food));
	Colony_Wood->SetText(FText::AsNumber(ColonyRes.Wood));
	Colony_Stone->SetText(FText::AsNumber(ColonyRes.Stone));

	FGameResources TribeRes = GameState->Tribe->GetResources();
	Tribe_Pop->SetText(FText::AsNumber(Natives));
	Tribe_Food->SetText(FText::AsNumber(TribeRes.Food));
	Tribe_Wood->SetText(FText::AsNumber(TribeRes.Wood));
	Tribe_Stone->SetText(FText::AsNumber(TribeRes.Stone));
}
