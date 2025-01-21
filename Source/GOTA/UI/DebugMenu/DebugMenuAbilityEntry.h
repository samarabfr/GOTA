#pragma once

#include "Blueprint/UserWidget.h"
#include "DebugMenuAbilityEntry.generated.h"

class UAbilitySettings;
class UTextBlock;
class UImage;

UCLASS()
class UDebugMenuAbilityEntry : public UUserWidget
{
	GENERATED_BODY()
	
	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	
	// ---------------------------------------- Utility ----------------------------------------
private:
	UPROPERTY(meta = (BindWidget))
	UImage* AbilityImage;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* AbilityName;

	TWeakObjectPtr<UAbilitySettings> AbilitySettings;

public:
	void SetAbility(UAbilitySettings* NewAbilitySettings);
	UAbilitySettings* GetAbility() const { return AbilitySettings.Get(); }
};
