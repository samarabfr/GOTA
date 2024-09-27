#pragma once


#include "Blueprint/UserWidget.h"
#include "IngameUI.generated.h"

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
	
	// --------------------------------------------------
	
private:
	virtual void NativeConstruct() override;

	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

public:
	UFUNCTION(BlueprintCallable)
	void ClickActor(AActor* Actor);
};
