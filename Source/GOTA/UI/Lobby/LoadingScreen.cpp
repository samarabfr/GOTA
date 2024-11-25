#include "LoadingScreen.h"

#include "LoadingProgress.h"
#include "GOTA/CoreSystems/GameplayFramework/GS_Ingame.h"
#include "GOTA/CoreSystems/GameplayFramework/LoadingManager.h"

void ULoadingScreen::NativeConstruct()
{
	Super::NativeConstruct();
	LoadingProgresses.Add(LoadingProgress1);
	LoadingProgresses.Add(LoadingProgress2);
	LoadingProgresses.Add(LoadingProgress3);
	LoadingProgresses.Add(LoadingProgress4);
}

void ULoadingScreen::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	const AGS_Ingame* GameState = GetWorld()->GetGameState<AGS_Ingame>();
	if (!GameState) return;

	ALoadingManager* LoadingManager = GameState->GetLoadingManager();
	if (!LoadingManager) return;

	for (ALoadingStatusActor* LoadingStatus : LoadingManager->LoadingStatuses)
	{
		// LoadingStatus is either nullptr or has an Invalid PlayerID
		if (!LoadingStatus || LoadingStatus->GOTAPlayerID < 0) continue;

		if (LoadingProgresses.IsValidIndex(LoadingStatus->GOTAPlayerID)
			&& !LoadingProgresses[LoadingStatus->GOTAPlayerID]->IsInitialized())
		{
			LoadingProgresses[LoadingStatus->GOTAPlayerID]->Init(LoadingStatus);
		}
	}
}
