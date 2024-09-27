#pragma once

#include "Blueprint/UserWidget.h"
#include "Clock.generated.h"

class AGS_Ingame;
class UImage;

UCLASS(Blueprintable)
class GOTA_API UClock : public UUserWidget
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

	// --------------------------------------------------
private:
	UPROPERTY()
	AGS_Ingame* GameState;

	virtual void NativeConstruct() override;

	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
};
