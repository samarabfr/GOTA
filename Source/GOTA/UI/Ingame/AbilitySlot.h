#pragma once

#include "Blueprint/UserWidget.h"
#include "AbilitySlot.generated.h"

class UCanvasPanel;
class AAbility;
class UAbilityWidget;
class UImage;

UCLASS(Blueprintable)
class GOTA_API UAbilitySlot : public UUserWidget
{
	GENERATED_BODY()

	// ---------------------------------------- Lifecycle ----------------------------------------
public:
	void Init(FName InSlotName);

	// ---------------------------------------- Utility ----------------------------------------
private:
	TWeakObjectPtr<UAbilityWidget> AbilityWidget;
	
	UPROPERTY(EditDefaultsOnly);
	TSubclassOf<class UAbilityWidget> AbilityWidgetClass;

	UPROPERTY(meta = (BindWidget))
	UCanvasPanel* Canvas;
	
public:
	void SetAbilityWidget(UAbilityWidget* NewAbilityWidget);

	void SpawnAbilityWidget(AAbility* Ability);

	AAbility* GetAbility() const;

	// ---------------------------------------- SlotName ----------------------------------------
private:
	FName SlotName;

public:
	FName GetSlotName() const { return SlotName; }
};
