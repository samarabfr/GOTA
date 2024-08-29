#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/ComboBoxString.h"
#include "Components/TextBlock.h"
#include "GOTA/CoreSystems/GameplayFramework/PS_Ingame.h"
#include "PlayerSlot.generated.h"

UCLASS()
class GOTA_API UPlayerSlot : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UPROPERTY(BlueprintReadWrite, Category="PlayerSlot")
	APS_Ingame* CachedPlayerState;

	UPROPERTY(meta = (BindWidget))
	UComboBoxString* GuardianSelection;
	
	UPROPERTY(meta = (BindWidget))
	UTextBlock* PlayerName;
	
	UPROPERTY(EditDefaultsOnly, Category="PlayerSlot")
	TArray<UGuardianDataAsset*> Guardians;
	
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category="PlayerSlot")
	void SetPlayerState(APS_Ingame* PlayerState);

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category="PlayerSlot")
	void UpdatePlayerState(APS_Ingame* PlayerState);
};
