// Fill out your copyright notice in the Description page of Project Settings.

#include "AbilityProvider.h"
#include "Ability.h"
#include "AbilityRegisterEntry.h"

// ---------------------------------------- Utility ----------------------------------------

void UAbilityProvider::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	static const TCHAR* PathToDataTable = TEXT("/Game/CoreSystems/Guardian/Abilities/DT_AbilityRegister");
	if (UDataTable* AbilityRegister = LoadObject<UDataTable>(nullptr, PathToDataTable))
	{
		AbilityRegister->ForeachRow<FAbilityRegisterEntry>(
			TEXT("Load Abilities"),
			[&](const FName& RowName, const FAbilityRegisterEntry& RowData)
			{
				AbilitySettings.Add(RowData.AbilitySettings);
			});
	}
	UE_LOG(LogTemp, Warning, TEXT("Loaded %d Abilities"), AbilitySettings.Num())
}