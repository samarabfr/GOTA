#pragma once

#include "Blueprint/UserWidget.h"
#include "DebugMenu.generated.h"

class UAbilitySettings;
class UDebugMenuAbilityEntry;
class UUniformGridPanel;

UCLASS()
class UDebugMenu : public UUserWidget
{
	GENERATED_BODY()

	// -------------------------------------- LifeCycle --------------------------------------

	virtual void NativeConstruct() override;

	// ---------------------------------------- Utility ----------------------------------------
private:
	UPROPERTY(EditDefaultsOnly)
	int32 GridColumns = 6;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UDebugMenuAbilityEntry> AbilityEntryClass;

	UPROPERTY(meta = (BindWidget))
	UUniformGridPanel* GrantAbilityGrid;

	int32 GridCounter = 0;

	void AddToGrid(UAbilitySettings* AbilitySettings);

	// ------------------------------------ Prevent Clicking Through ------------------------------------

	virtual FReply NativeOnMouseButtonDown
	(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;

	virtual FReply NativeOnMouseButtonDoubleClick
	(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
};
