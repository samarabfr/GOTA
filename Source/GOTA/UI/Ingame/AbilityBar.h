#pragma once

#include "Blueprint/UserWidget.h"
#include "Abilitybar.generated.h"

class UAbilitySlot;

UCLASS(Blueprintable)
class GOTA_API UAbilityBar : public UUserWidget
{
	GENERATED_BODY()
	
	// -------------------Widgets------------------------
protected:
	UPROPERTY(meta = (BindWidget))
	UAbilitySlot* AbilitySlot1;

	UPROPERTY(meta = (BindWidget))
	UAbilitySlot* AbilitySlot2;

	UPROPERTY(meta = (BindWidget))
	UAbilitySlot* AbilitySlot3;
	
	UPROPERTY(meta = (BindWidget))
	UAbilitySlot* AbilitySlot4;

	UPROPERTY(meta = (BindWidget))
	UAbilitySlot* AbilitySlot5;

	UPROPERTY(meta = (BindWidget))
	UAbilitySlot* AbilitySlot6;
	
	UPROPERTY(meta = (BindWidget))
	UAbilitySlot* AbilitySlot7;

	UPROPERTY(meta = (BindWidget))
	UAbilitySlot* AbilitySlot8;

	// --------------------------------------------------
private:
	virtual void NativeConstruct() override;

	TArray<UAbilitySlot*> AbilitySlots;

public:
	TArray<UAbilitySlot*> GetAbilitySlots();
};
