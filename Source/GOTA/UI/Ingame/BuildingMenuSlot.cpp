#include "BuildingMenuSlot.h"

#include "Components/TextBlock.h"
#include "GOTA/Tile/Building/BuildingSettings.h"
#include "GOTA/GameplayFramework/PC_Ingame.h"

void UBuildingMenuSlot::NativeConstruct()
{
	Super::NativeConstruct();
}

void UBuildingMenuSlot::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
}

FReply UBuildingMenuSlot::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	if (APC_Ingame* PlayerController = GetWorld()->GetFirstPlayerController<APC_Ingame>())
	{
		PlayerController->StartPlacingBuilding(BuildingSettings);
	}
	return FReply::Handled();
}

FReply UBuildingMenuSlot::NativeOnMouseButtonDoubleClick(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	return FReply::Handled();
}

void UBuildingMenuSlot::SetBuildingSettings(UBuildingSettings* NewBuildingSettings)
{
	if (!NewBuildingSettings) return;
	BuildingSettings = NewBuildingSettings;
	Txt_Name->SetText(FText::FromName(NewBuildingSettings->Name));
	Txt_Description->SetText(NewBuildingSettings->Description);
}
