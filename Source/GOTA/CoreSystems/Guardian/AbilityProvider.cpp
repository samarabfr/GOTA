// Fill out your copyright notice in the Description page of Project Settings.

#include "AbilityProvider.h"
#include "AbilityRegisterEntry.h"
#include "GOTA/CoreSystems/GameplayFramework/GOTAGameInstance.h"

// ---------------------------------------- Utility ----------------------------------------

void UAbilityProvider::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	
	const UGOTAGameInstance* GameInstance = Cast<UGOTAGameInstance>(GetGameInstance());

	if (!GameInstance->GetAbilityRegister()) return;
	GameInstance->GetAbilityRegister()->ForeachRow<FAbilityRegisterEntry>(
		TEXT("Load Abilities"),
		[&](const FName& RowName, const FAbilityRegisterEntry& RowData)
		{
			AbilitySettings.Add(RowData.AbilitySettings);
		});
	UE_LOG(LogTemp, Warning, TEXT("Loaded %d Abilities"), AbilitySettings.Num())
}