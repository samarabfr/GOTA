#include "BuildingMenu.h"

#include "BuildingMenuSlot.h"
#include "Components/WrapBox.h"
#include "GOTA/GameplayFramework/PC_Ingame.h"
#include "GOTA/Guardian/Guardian.h"

void UBuildingMenu::NativeConstruct()
{
	Super::NativeConstruct();
	APC_Ingame* PlayerController = Cast<APC_Ingame>(GetOwningPlayer());
	if (!PlayerController)	return;
	RefreshMenuSlots(PlayerController->GetGuardian());
	PlayerController->OnGuardianChanged.AddDynamic(this, &UBuildingMenu::RefreshMenuSlots);
}

void UBuildingMenu::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
}

void UBuildingMenu::RefreshMenuSlots(AGuardian* Guardian)
{
	if (!Guardian) return;
	for (UBuildingSettings* Building : Guardian->GetPossibleBuildings())
	{
		if (!Building) continue;
		UBuildingMenuSlot* NewSlot = CreateWidget<UBuildingMenuSlot>(this, SlotClass);
		NewSlot->SetBuildingSettings(Building);
		WrapBox->AddChild(NewSlot);
		MenuSlots.Add(NewSlot);
	}
}
