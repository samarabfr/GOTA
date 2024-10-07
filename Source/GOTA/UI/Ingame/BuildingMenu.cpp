#include "BuildingMenu.h"

#include "BuildingMenuSlot.h"
#include "Components/WrapBox.h"

void UBuildingMenu::NativeConstruct()
{
	Super::NativeConstruct();
	for (UBuildingSettings* Building : PlaceableBuildings)
	{
		if (!Building) continue;
		UBuildingMenuSlot* NewSlot = CreateWidget<UBuildingMenuSlot>(this, SlotClass);
		NewSlot->SetBuildingSettings(Building);
		WrapBox->AddChild(NewSlot);
		MenuSlots.Add(NewSlot);
	}
}

void UBuildingMenu::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
}
