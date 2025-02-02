#pragma once

#include "Blueprint/UserWidget.h"
#include "DebugMenuAbilityGranter.generated.h"

class UAbilitySettings;
class UUniformGridPanel;
class UDebugMenuAbilityEntry;

UCLASS()
class GOTA_API UDebugMenuAbilityGranter : public UUserWidget
{
	GENERATED_BODY()

	// -------------------------------------------- LifeCycle --------------------------------------------
protected:
	virtual void NativeConstruct() override;

	// -------------------------------------------- Utility --------------------------------------------
private:
	UPROPERTY(EditDefaultsOnly)
	int32 GridColumns = 1;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UDebugMenuAbilityEntry> AbilityEntryClass;

	UPROPERTY(meta = (BindWidget))
	UUniformGridPanel* GrantAbilityGrid;

	int32 GridCounter = 0;

	void AddToGrid(UAbilitySettings* AbilitySettings);
};
