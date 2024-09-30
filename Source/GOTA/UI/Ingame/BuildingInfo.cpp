#include "BuildingInfo.h"

#include "Components/HorizontalBox.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"
#include "GOTA/CoreSystems/Faction/Building/Building.h"
#include "GOTA/CoreSystems/Faction/Building/BuildingSettings.h"
#include "GOTA/CoreSystems/Faction/Building/Population.h"

void UBuildingInfo::NativeConstruct()
{
	Super::NativeConstruct();
}

void UBuildingInfo::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
	if (!CurrentBuilding) return;
	if (CurrentBuilding->GetIsUnderConstruction())
	{
		PopulationBox->SetVisibility(ESlateVisibility::Collapsed);
		MoodletsBox->SetVisibility(ESlateVisibility::Collapsed);
		ConstructionBox->SetVisibility(ESlateVisibility::Visible);
		RefreshConstruction();
	}
	else
	{
		PopulationBox->SetVisibility(ESlateVisibility::Visible);
		MoodletsBox->SetVisibility(ESlateVisibility::Visible);
		ConstructionBox->SetVisibility(ESlateVisibility::Collapsed);
		RefreshPopulation();
	}
}

void UBuildingInfo::RefreshPopulation()
{
	UPopulation* Pop = CurrentBuilding->Population;
	Population_Current->SetText(FText::AsNumber(Pop->GetSize()));
	Population_Max->SetText(FText::AsNumber(Pop->GetMaxSize()));
	Population_Growth->SetText(FText::Format(FText::FromString(TEXT("+{0}")), Pop->GetGrowth()));
	Population_Progress->SetPercent(Pop->GetGrowthProgress() * 0.01f);

	Mood_Angry->SetText(FText::AsNumber(Pop->GetAngry()));
	Mood_Fear->SetText(FText::AsNumber(Pop->GetFear()));
}

void UBuildingInfo::RefreshConstruction()
{
	FGameResources Progress = CurrentBuilding->GetResourceProgress();
	FGameResources Cost = CurrentBuilding->Settings->Cost;
	Wood_Current->SetText(FText::AsNumber(Progress.Wood));
	Wood_Target->SetText(FText::AsNumber(Cost.Wood));
	Stone_Current->SetText(FText::AsNumber(Progress.Stone));
	Stone_Target->SetText(FText::AsNumber(Cost.Stone));
}

void UBuildingInfo::WatchBuilding(UBuilding* Building)
{
	CurrentBuilding = Building;
	if (!Building) return;
	BuildingName->SetText(FText::FromName(CurrentBuilding->Settings->Name));
}
