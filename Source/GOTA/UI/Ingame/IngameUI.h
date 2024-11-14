#pragma once


#include "Blueprint/UserWidget.h"
#include "GOTA/CoreSystems/Utility/Enums.h"
#include "IngameUI.generated.h"

class AGS_Ingame;
class UBuildingMenu;
class UButton;
class UGuardianInfo;
class UTextBlock;
class ACombat;
class UClickedInfo;
class USpinBox;

UCLASS(Blueprintable)
class GOTA_API UIngameUI : public UUserWidget
{
	GENERATED_BODY()

	// -------------------Widgets------------------------
protected:
	UPROPERTY(meta = (BindWidget))
	UClickedInfo* ClickedInfo;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* Txt_GameEnding;

	UPROPERTY(meta = (BindWidget))
	UGuardianInfo* GuardianInfo1;

	UPROPERTY(meta = (BindWidget))
	UGuardianInfo* GuardianInfo2;

	UPROPERTY(meta = (BindWidget))
	UGuardianInfo* GuardianInfo3;

	UPROPERTY(meta = (BindWidget))
	UGuardianInfo* GuardianInfo4;


	// --------------------------------------------------

private:
	virtual void NativeConstruct() override;

	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	UFUNCTION()
	void RefreshGuardianWidgets(AGS_Ingame* GameState);

public:
	UFUNCTION(BlueprintCallable)
	void ClickActor(AActor* Actor);

	void HoverActor(AActor* Actor);

	UFUNCTION(BlueprintCallable)
	void WatchCombat(ACombat* Combat);

	UFUNCTION()
	void OnGameEnding(const EGameEnding Ending, const FString& EndingMessage);

	// ------------------------------- Build Menu -------------------------------
public:
	UFUNCTION()
	void ToggleBuildMenu();

protected:
	UPROPERTY(meta = (BindWidget))
	UButton* Btn_Build;

	UPROPERTY(meta = (BindWidget))
	UBuildingMenu* BuildingMenu;

private:
	void CloseBuildMenu();
	void OpenBuildMenu();
};
