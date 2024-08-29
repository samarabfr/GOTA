#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/ComboBoxString.h"
#include "Components/TextBlock.h"
#include "GOTA/CoreSystems/GameplayFramework/PS_Ingame.h"
#include "PlayerSlot.generated.h"

UCLASS(Blueprintable)
class GOTA_API UPlayerSlot : public UUserWidget
{
	GENERATED_BODY()
	bool IsLocalPlayerState = false;

public:
	UPROPERTY(BlueprintReadWrite, Category="PlayerSlot")
	APS_Ingame* CachedPlayerState;

	UPROPERTY(meta = (BindWidget))
	UComboBoxString* GuardianSelection;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* PlayerName;

	UPROPERTY(EditDefaultsOnly, Category="PlayerSlot")
	TArray<UGuardianDataAsset*> Guardians;

	virtual void NativeConstruct() override;
	
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
	
	UFUNCTION(BlueprintCallable, Category="PlayerSlot")
	void SetPlayerState(APS_Ingame* PlayerState);
	
	UFUNCTION()
	void OnSelectionChanged(FString SelectedItem, ESelectInfo::Type SelectionType);

private:
	UGuardianDataAsset* GetSelectedGuardian() const;
};
