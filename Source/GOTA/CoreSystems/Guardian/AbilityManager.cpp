// Fill out your copyright notice in the Description page of Project Settings.

#include "AbilityManager.h"
#include "AbilityRegisterEntry.h"

void UAbilityManager::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	static const TCHAR* PathToDataTable = TEXT("/Game/CoreSystems/Guardian/Abilities/DT_AbilityRegister");
	if (UDataTable* AbilityRegister = LoadObject<UDataTable>(nullptr, PathToDataTable))
	{
		AbilityRegister->ForeachRow<FAbilityRegisterEntry>(
			TEXT("Load Abilities"),
			[&](const FName& RowName, const FAbilityRegisterEntry& RowData)
			{
				Abilities.Add(RowData.AbilitySettings);
			});
	}
	UE_LOG(LogTemp, Warning, TEXT("Loaded %d Abilities"), Abilities.Num())
}
