#pragma once

#include "Blueprint/UserWidget.h"
#include "BuildingMenu.generated.h"

class UWrapBox;
class UBuildingSettings;
class UBuildingMenuSlot;

UCLASS(Blueprintable)
class GOTA_API UBuildingMenu : public UUserWidget
{
	GENERATED_BODY()
	
	// -------------------Widgets------------------------
protected:
	UPROPERTY(meta = (BindWidget))
	UWrapBox* WrapBox;
	
	// --------------------------------------------------
private:
	virtual void NativeConstruct() override;

	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	TArray<UBuildingMenuSlot*> MenuSlots;

protected:
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UBuildingMenuSlot> SlotClass;

	UPROPERTY(EditDefaultsOnly)
	TArray<UBuildingSettings*> PlaceableBuildings;
};
