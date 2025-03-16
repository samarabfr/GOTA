// Fill out your copyright notice in the Description page of Project Settings.

#include "AbilityProvider.h"
#include "AbilityRegisterEntry.h"
#include "GOTA/GameplayFramework/GotaGameInstance.h"

// ---------------------------------------- Utility ----------------------------------------

void UAbilityProvider::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	
	const UGotaGameInstance* GameInstance = Cast<UGotaGameInstance>(GetGameInstance());

	if (!GameInstance->GetAbilityRegister()) return;
	GameInstance->GetAbilityRegister()->ForeachRow<FAbilityRegisterEntry>(
		TEXT("Load Abilities"),
		[&](const FName& RowName, const FAbilityRegisterEntry& RowData)
		{
			AbilitySettings.Add(RowData.AbilitySettings);
		});
	UE_LOG(LogTemp, Warning, TEXT("Loaded %d Abilities"), AbilitySettings.Num())
}