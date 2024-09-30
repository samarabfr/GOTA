#include "BuildingMenuSlot.h"

#include "Components/TextBlock.h"
#include "GOTA/CoreSystems/Faction/Building/BuildingSettings.h"

void UBuildingMenuSlot::NativeConstruct()
{
	Super::NativeConstruct();
}

void UBuildingMenuSlot::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
}

void UBuildingMenuSlot::SetBuildingSettings(UBuildingSettings* NewBuildingSettings)
{
	if (!NewBuildingSettings) return;
	BuildingSettings = NewBuildingSettings;
	Txt_Name->SetText(FText::FromName(NewBuildingSettings->Name));
	Txt_Description->SetText(NewBuildingSettings->Description);
}
