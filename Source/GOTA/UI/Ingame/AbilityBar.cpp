#include "AbilityBar.h"

#include "AbilitySlot.h"

void UAbilityBar::NativeConstruct()
{
	Super::NativeConstruct();
	AbilitySlots.Add(AbilitySlot1);
	AbilitySlot1->Init(FName("AbilityBar1"));
	
	AbilitySlots.Add(AbilitySlot2);
	AbilitySlot2->Init(FName("AbilityBar2"));
	
	AbilitySlots.Add(AbilitySlot3);
	AbilitySlot3->Init(FName("AbilityBar3"));
	
	AbilitySlots.Add(AbilitySlot4);
	AbilitySlot4->Init(FName("AbilityBar4"));
	
	AbilitySlots.Add(AbilitySlot5);
	AbilitySlot5->Init(FName("AbilityBar5"));
	
	AbilitySlots.Add(AbilitySlot6);
	AbilitySlot6->Init(FName("AbilityBar6"));
	
	AbilitySlots.Add(AbilitySlot7);
	AbilitySlot7->Init(FName("AbilityBar7"));
	
	AbilitySlots.Add(AbilitySlot8);
	AbilitySlot8->Init(FName("AbilityBar8"));
}

TArray<UAbilitySlot*> UAbilityBar::GetAbilitySlots()
{
	return AbilitySlots;
}
