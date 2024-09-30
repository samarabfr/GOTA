#pragma once

#include "Blueprint/UserWidget.h"
#include "BuildingInfo.generated.h"

class UHorizontalBox;
class UBuilding;
class UProgressBar;
class UTextBlock;

UCLASS(Blueprintable)
class GOTA_API UBuildingInfo : public UUserWidget
{
	GENERATED_BODY()
	
	// -------------------Widgets------------------------
protected:
	UPROPERTY(meta = (BindWidget))
	UTextBlock* BuildingName;

	UPROPERTY(meta = (BindWidget))
	UHorizontalBox* PopulationBox;
	
	UPROPERTY(meta = (BindWidget))
	UTextBlock* Population_Current;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* Population_Max;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* Population_Growth;

	UPROPERTY(meta = (BindWidget))
	UProgressBar* Population_Progress;

	UPROPERTY(meta = (BindWidget))
	UHorizontalBox* MoodletsBox;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* Mood_Angry;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* Mood_Fear;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* Mood_Sick;
	
	UPROPERTY(meta = (BindWidget))
	UHorizontalBox* ConstructionBox;
	
	UPROPERTY(meta = (BindWidget))
	UTextBlock* Wood_Current;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* Wood_Target;
	
	UPROPERTY(meta = (BindWidget))
	UTextBlock* Stone_Current;
	
	UPROPERTY(meta = (BindWidget))
	UTextBlock* Stone_Target;
	
	// ---------------------------------------------------------
private:	
	UPROPERTY()
	UBuilding* CurrentBuilding;
	
	virtual void NativeConstruct() override;

	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	void RefreshPopulation();

	void RefreshConstruction();
	
public:
	void WatchBuilding(UBuilding* Building);
};
