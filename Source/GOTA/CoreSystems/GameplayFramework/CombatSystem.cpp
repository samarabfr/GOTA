#include "CombatSystem.h"

#include "GOTA/CoreSystems/Faction/Building/Building.h"
#include "GOTA/CoreSystems/Faction/Settlement/Settlement.h"
#include "GOTA/CoreSystems/Tile/Tile.h"

void UCombatSystem::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	UObject::GetLifetimeReplicatedProps(OutLifetimeProps);
}

bool UCombatSystem::IsSupportedForNetworking() const
{
	return true;
}

UCombatSystem::UCombatSystem()
{
	// Load GameBalance for Combat Value calculation
	static ConstructorHelpers::FObjectFinder<UGameBalanceDataAsset> DataAsset2(
		TEXT("/Game/CoreSystems/GameplayFramework/DA_GameBalance"));
	if (DataAsset2.Succeeded())
	{
		GameBalance = DataAsset2.Object;
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Combat System couldn't load GameBalance Data Asset"))
	}
}

void UCombatSystem::RegisterCombat(ATile* Tile)
{
	CurrentCombatSources.Add(Tile);
}

void UCombatSystem::TriggerAllCombats()
{
	for (ATile* CombatSource : CurrentCombatSources)
	{
		EvaluateCombat(CombatSource);
	}
	CurrentCombatSources.Empty();
}

void UCombatSystem::EvaluateCombat(ATile* Tile)
{
	int32 EnemyAttack = 0;
	int32 EnemyDefense = 0;
	CalcCombatValues(Tile, EnemyAttack, EnemyDefense, EAffiliation::Enemy);
	int32 AllyAttack = 0;
	int32 AllyDefense = 0;
	CalcCombatValues(Tile, AllyAttack, AllyDefense, EAffiliation::Ally);
	int32 EnemyDamage = EnemyAttack - AllyDefense;
	int32 AllyDamage = AllyAttack - EnemyDefense;


	Tile->Unbuild();
	if (Tile->GetEnemyEntity())
		Tile->GetEnemyEntity()->Kill();
	if (Tile->GetAlliedEntity())
		Tile->GetAlliedEntity()->Kill();
}

void UCombatSystem::CalcCombatValues(ATile* Tile, int32& Attack, int32& Defense, EAffiliation Affiliation)
{
	if (Affiliation == EAffiliation::Enemy && Tile->GetEnemyEntity())
	{
		Attack += Tile->GetEnemyEntity()->GetAttack();
		Defense += Tile->GetEnemyEntity()->GetDefense();
	}
	if (Affiliation == EAffiliation::Ally && Tile->GetAlliedEntity())
	{
		Attack += Tile->GetAlliedEntity()->GetAttack();
		Defense += Tile->GetAlliedEntity()->GetDefense();
	}
	if(Tile->Building && Tile->GetClaimant() && Tile->GetClaimant()->Affiliation == Affiliation)
	{
		Attack += Tile->Building->PopContainer->CombatValues->GetAttack();
		Defense += Tile->Building->PopContainer->CombatValues->GetDefense();
	}
}
