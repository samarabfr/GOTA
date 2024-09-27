#include "Clock.h"

#include "Components/Image.h"
#include "GOTA/CoreSystems/GameplayFramework/GS_Ingame.h"


void UClock::NativeConstruct()
{
	Super::NativeConstruct();
	GameState = GetWorld()->GetGameState<AGS_Ingame>();
}

void UClock::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
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
}
