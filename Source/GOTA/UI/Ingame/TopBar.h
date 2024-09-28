#pragma once

#include "Blueprint/UserWidget.h"
#include "TopBar.generated.h"

class UTextBlock;
class AGS_Ingame;
class UImage;

UCLASS(Blueprintable)
class GOTA_API UTopBar : public UUserWidget
{
	GENERATED_BODY()


	// -------------------Widgets------------------------
protected:
	UPROPERTY(meta = (BindWidget))
	UImage* Daytime_Disk;

	UPROPERTY(meta = (BindWidget))
	UImage* Health_Disk;

	UPROPERTY(meta = (BindWidget))
	UImage* Power_Disk;
	
	UPROPERTY(meta = (BindWidget))
	UTextBlock* Colony_Pop;
	
	UPROPERTY(meta = (BindWidget))
	UTextBlock* Colony_Food;
	
	UPROPERTY(meta = (BindWidget))
	UTextBlock* Colony_Wood;
	
	UPROPERTY(meta = (BindWidget))
	UTextBlock* Colony_Stone;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* Tribe_Pop;
	
	UPROPERTY(meta = (BindWidget))
	UTextBlock* Tribe_Food;
	
	UPROPERTY(meta = (BindWidget))
	UTextBlock* Tribe_Wood;
	
	UPROPERTY(meta = (BindWidget))
	UTextBlock* Tribe_Stone;
	
	// --------------------------------------------------
private:
	UPROPERTY()
	AGS_Ingame* GameState;

	virtual void NativeConstruct() override;

	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
};
