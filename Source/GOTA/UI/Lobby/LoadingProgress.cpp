#include "LoadingProgress.h"

#include "Components/TextBlock.h"
#include "GOTA/CoreSystems/GameplayFramework/LoadingStatusActor.h"


void ULoadingProgress::Init(ALoadingStatusActor* InLoadingStatus)
{
	LoadingStatus = InLoadingStatus;
	if (LoadingStatus)
	{
		PlayerID->SetText(FText::AsNumber(LoadingStatus->GOTAPlayerID));
	}
}

void ULoadingProgress::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
	if (LoadingStatus)
	{
		const UEnum* EnumPtr = FindObject<UEnum>(ANY_PACKAGE, TEXT("ELoadingStatus"), true);
		if (EnumPtr)
		{
			const FText StatusText = EnumPtr->GetDisplayNameTextByValue(
				static_cast<int64>(LoadingStatus->CurrentStatus));
			Status->SetText(StatusText);
		}
		ReplicationCount->SetText(FText::AsNumber(LoadingStatus->NetRepCount));
	}
}
