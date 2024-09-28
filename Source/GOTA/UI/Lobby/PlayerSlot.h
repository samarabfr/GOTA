#pragma once

#include "Blueprint/UserWidget.h"
#include "PlayerSlot.generated.h"

class UGuardianSettings;
class UTextBlock;
class UComboBoxString;

UCLASS(Blueprintable)
class GOTA_API UPlayerSlot : public UUserWidget
{
	GENERATED_BODY()
	bool IsLocalPlayerState = false;

public:
	TWeakObjectPtr<APS_Ingame>  CachedPlayerState;
	
	UPROPERTY(meta = (BindWidget))
	UComboBoxString* GuardianSelection;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* PlayerName;

	UPROPERTY(EditDefaultsOnly, Category="PlayerSlot")
	TArray<UGuardianSettings*> Guardians;

	virtual void NativeConstruct() override;
	
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
	
	UFUNCTION(BlueprintCallable, Category="PlayerSlot")
	void SetPlayerState(APS_Ingame* PlayerState);
	
	UFUNCTION()
	void OnSelectionChanged(FString SelectedItem, ESelectInfo::Type SelectionType);

private:
	UGuardianSettings* GetSelectedGuardian() const;
};
