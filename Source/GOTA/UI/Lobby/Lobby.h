#pragma once

#include "CoreMinimal.h"
#include "PlayerSlot.h"
#include "Components/Button.h"
#include "Components/SpinBox.h"
#include "Blueprint/UserWidget.h"
#include "GOTA/CoreSystems/GameplayFramework/GS_Ingame.h"
#include "Lobby.generated.h"

UCLASS(Blueprintable)
class GOTA_API ULobby : public UUserWidget
{
	GENERATED_BODY()

	UPROPERTY()
	AGS_Ingame* GameState;

	UPROPERTY()
	TArray<UPlayerSlot*> PlayerSlots;

public:
	// ---------------------------------------------------------
	// Widgets

	UPROPERTY(meta = (BindWidget))
	UPlayerSlot* PlayerSlot1;

	UPROPERTY(meta = (BindWidget))
	UPlayerSlot* PlayerSlot2;

	UPROPERTY(meta = (BindWidget))
	UPlayerSlot* PlayerSlot3;

	UPROPERTY(meta = (BindWidget))
	UPlayerSlot* PlayerSlot4;

	UPROPERTY(meta = (BindWidget))
	UButton* BTN_Start;

	UPROPERTY(meta = (BindWidget))
	UButton* BTN_Leave;

	UPROPERTY(meta = (BindWidget))
	USpinBox* SB_IslandSize;
	
	// ---------------------------------------------------------
	// 
private:
	virtual void NativeConstruct() override;

	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	UFUNCTION()
	void StartPressed();
	
	UFUNCTION()
	void LeavePressed();

	UFUNCTION()
	void IslandTilesChanged(float InValue, ETextCommit::Type CommitMethod);

	UFUNCTION()
	void StartParameterChanged(UStartParameter* StartParameter);
};
