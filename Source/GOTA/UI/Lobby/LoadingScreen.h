#pragma once

#include "Blueprint/UserWidget.h"
#include "LoadingScreen.generated.h"

class ULoadingProgress;

UCLASS(Blueprintable)
class GOTA_API ULoadingScreen : public UUserWidget
{
	GENERATED_BODY()
	
	// ------------------- LifeCycle -------------------
	
	virtual void NativeConstruct() override;
	
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	// ------------------- Utility -------------------

	TArray<ULoadingProgress*> LoadingProgresses;
	
	// ------------------- Widgets -------------------
	
	UPROPERTY(meta = (BindWidget))
	ULoadingProgress* LoadingProgress1;

	UPROPERTY(meta = (BindWidget))
	ULoadingProgress* LoadingProgress2;

	UPROPERTY(meta = (BindWidget))
	ULoadingProgress* LoadingProgress3;

	UPROPERTY(meta = (BindWidget))
	ULoadingProgress* LoadingProgress4;
};
