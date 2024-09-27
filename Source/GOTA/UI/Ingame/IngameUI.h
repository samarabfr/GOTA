#pragma once


#include "Blueprint/UserWidget.h"
#include "GOTA/CoreSystems/Utility/Enums.h"
#include "IngameUI.generated.h"

class UTextBlock;
class ACombat;
class UClickedInfo;
class USpinBox;

UCLASS(Blueprintable)
class GOTA_API UIngameUI : public UUserWidget
{
	GENERATED_BODY()

public:
	// -------------------Widgets------------------------
	
	UPROPERTY(meta = (BindWidget))
	UClickedInfo* ClickedInfo;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* Txt_GameEnding;
	
	// --------------------------------------------------
	
private:
	virtual void NativeConstruct() override;

	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

public:
	UFUNCTION(BlueprintCallable)
	void ClickActor(AActor* Actor);

	void HoverActor(AActor* Actor);
	
	UFUNCTION(BlueprintCallable)
	void WatchCombat(ACombat* Combat);

	UFUNCTION()
	void OnGameEnding(const EGameEnding Ending, const FString& EndingMessage);
};
