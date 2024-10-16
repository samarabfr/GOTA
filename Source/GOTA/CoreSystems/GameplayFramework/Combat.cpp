#include "Combat.h"

#include "GOTA/CoreSystems/Faction/Building/Building.h"
#include "GOTA/CoreSystems/Faction/Settlement/Settlement.h"
#include "GOTA/CoreSystems/Tile/Tile.h"

void ACombat::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
}

ACombat::ACombat()
{
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;

	RootComponent = CreateDefaultSubobject<USceneComponent>("ROOT");
	MainMesh = CreateDefaultSubobject<UStaticMeshComponent>("Main Mesh");
	MainMesh->SetupAttachment(RootComponent);

	ConstructorHelpers::FObjectFinder<UGameBalanceDataAsset> DataAssetFinder(
	TEXT("/Game/CoreSystems/GameplayFramework/DA_GameBalance"));
	GameBalance = DataAssetFinder.Object;
}

void ACombat::BeginPlay()
{
	Super::BeginPlay();
}

bool ACombat::DoesCombatTilesContain(ATile* Tile)
{
	for (FCombatTile& CombatTile : CombatTiles)
	{
		if (CombatTile.Tile == Tile) return true;
	}
	return false;
}

void ACombat::AddCombatTile(FCombatTile CombatTile)
{
	CombatTiles.Add(CombatTile);
	// anfangen zu tracken
	//CombatTile.Tile->OnEntityChanged.AddDynamic(this, &ACombat::EntityChanged);
	EntityChanged(CombatTile.Tile, nullptr);
	CombatTile.Tile->OnBuildingChanged.AddDynamic(this, &ACombat::BuildingChanged);
	BuildingChanged(CombatTile.Tile);
	// Add graphic to show that this tile belongs to this Combat
	UStaticMeshComponent* SMC = NewObject<UStaticMeshComponent>(this);
	SMC->AttachToComponent(RootComponent, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
	SMC->SetStaticMesh(SmallCombatMesh);
	SMC->RegisterComponent();
	SMC->SetWorldLocation(CombatTile.Tile->GetActorLocation() + FVector(0, 0, 600));
	SMC->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void ACombat::RemoveCombatTile(FCombatTile CombatTile)
{
	CombatTiles.Remove(CombatTile);
	// aufhören zu tracken
	//CombatTile.Tile->OnEntityChanged.RemoveDynamic(this, &ACombat::EntityChanged);
	CombatTile.Tile->OnBuildingChanged.RemoveDynamic(this, &ACombat::BuildingChanged);
}

void ACombat::AddSource(ATile* Tile)
{
	PreventCalcKills = true;
	if (CombatTiles.IsEmpty())
		SetActorLocation(Tile->GetActorLocation());
	if (!DoesCombatTilesContain(Tile))
	{
		AddCombatTile(FCombatTile(Tile));
	}
	for (ATile* Neighbor : Tile->Neighbors)
	{
		if (!Neighbor || DoesCombatTilesContain(Neighbor)) continue;
		AddCombatTile(FCombatTile(Neighbor));
	}
	PreventCalcKills = false;
	CalcKills();
}

bool ACombat::ShouldMerge(ATile* Tile)
{
	if (DoesCombatTilesContain(Tile)) return true;
	for (ATile* Neighbor : Tile->Neighbors)
	{
		if (DoesCombatTilesContain(Neighbor)) return true;
	}
	return false;
}

void ACombat::EntityChanged(ATile* Tile, AEntity* OldEntity)
{
	/*
	CalcKills();
	// unregister from delegates
	if (OldEntity) OldEntity->OnCombatValuesChanged.RemoveDynamic(this, &ACombat::CombatValuesChanged);
	// register to delegates
	if (Tile->GetAlliedEntity())
		Tile->GetAlliedEntity()->OnCombatValuesChanged.AddDynamic(
			this, &ACombat::CombatValuesChanged);
	if (Tile->GetEnemyEntity())
		Tile->GetEnemyEntity()->OnCombatValuesChanged.AddDynamic(
			this, &ACombat::CombatValuesChanged);
			*/
}

void ACombat::BuildingChanged(ATile* Tile)
{
	CalcKills();
	// register to delegates, unregister not necessary
	if (!Tile->GetBuilding()) return;
	//	Tile->Building->PopContainer->CombatValues->OnChanged.AddDynamic(this, &ACombat::CombatValuesChanged);
}

void ACombat::CombatValuesChanged(UCombatValues* CombatValues)
{
	CalcKills();
}

void ACombat::CalcKills()
{
	if (PreventCalcKills) return;
	CalcAttackDefense();
	AlliedDamage = AlliedAttack - EnemyDefense;
	EnemyDamage = EnemyAttack - AlliedDefense;
	for (FCombatTile& CombatTile : CombatTiles)
	{
		CombatTile.ZeroNumbers();
	}
	SpreadDamage(AlliedDamage, EAffiliation::Enemy);
	SpreadDamage(EnemyDamage, EAffiliation::Ally);
	CountKills();
}

void ACombat::CalcAttackDefense()
{
	/*
	AlliedAttack = 0;
	AlliedDefense = 0;
	EnemyAttack = 0;
	EnemyDefense = 0;
	for (FCombatTile& CombatTile : CombatTiles)
	{
		if (CombatTile.Tile->GetAlliedEntity())
		{
			UCombatValues* CV = CombatTile.Tile->GetAlliedEntity()->GetCombatValues();
			AlliedAttack += CV->GetAttack();
			AlliedDefense += CV->GetDefense();
		}
		if (CombatTile.Tile->GetEnemyEntity())
		{
			UCombatValues* CV = CombatTile.Tile->GetEnemyEntity()->GetCombatValues();
			EnemyAttack += CV->GetAttack();
			EnemyDefense += CV->GetDefense();
		}
		if (CombatTile.Tile->Building
			&& CombatTile.Tile->GetClaimant())
		{
			UCombatValues* CV = CombatTile.Tile->Building->GetCombatValues();
			if (CombatTile.Tile->GetClaimant()->Affiliation == EAffiliation::Ally)
			{
				AlliedAttack += CV->GetAttack();
				AlliedDefense += CV->GetDefense();
			}
			else
			{
				EnemyAttack += CV->GetAttack();
				EnemyDefense += CV->GetDefense();
			}
		}
	}
		*/
}

void ACombat::SpreadDamage(int32 Damage, EAffiliation Receiver)
{
	SpreadDamageToEntities(Receiver, Damage);
	SpreadDamageToBuildingPop(Receiver, Damage);
	SpreadDamageToBuildings(Receiver, Damage);
}

void ACombat::SpreadDamageToEntities(EAffiliation Receiver, int32& DamageLeft)
{
	/*
	if (DamageLeft <= 0) return;
	// get combat tiles with Receiver entities
	TArray<FCombatTile*> EntityCombatTiles;
	int32 TotalHP = 0;
	for (FCombatTile& CombatTile : CombatTiles)
	{
		if (CombatTile.Tile->GetEntity(Receiver))
		{
			EntityCombatTiles.Add(&CombatTile);
			UCombatValues* CV = CombatTile.Tile->GetEntity(Receiver)->GetCombatValues();
			TotalHP += CV->GetHP();
		}
	}
	if (TotalHP <= 0) return;
	// entities
	int32 Damage = DamageLeft;
	// spread damage based on total hp ratio
	for (FCombatTile* EntityCombatTile : EntityCombatTiles)
	{
		UCombatValues* CV = EntityCombatTile->Tile->GetEntity(Receiver)->GetCombatValues();
		int32 EntityDamage = CV->GetHP() / TotalHP * Damage;
		int32 EntityKills = FMath::Min(EntityDamage / CV->GetIndividualHP(), CV->GetIndividuals());
		EntityCombatTile->SetEntityKills(EntityKills, Receiver);
		DamageLeft -= EntityKills * CV->GetIndividualHP();
	}
	// spread left over damage
	EntityCombatTiles.Sort([&](const FCombatTile& A, const FCombatTile& B)
	{
		return A.Tile->GetEntity(Receiver)->GetCombatValues()->GetIndividualHP()
			> B.Tile->GetEntity(Receiver)->GetCombatValues()->GetIndividualHP();
	});
	bool HasKilledSomething = true;
	while (HasKilledSomething)
	{
		HasKilledSomething = false;
		for (FCombatTile* EntityCombatTile : EntityCombatTiles)
		{
			UCombatValues* CV = EntityCombatTile->Tile->GetEntity(Receiver)->GetCombatValues();
			if (CV->GetIndividuals() > EntityCombatTile->GetEntityKills(Receiver)
				&& DamageLeft >= CV->GetIndividualHP())
			{
				HasKilledSomething = true;
				EntityCombatTile->SetEntityKills(EntityCombatTile->GetEntityKills(Receiver) + 1, Receiver);
				DamageLeft -= CV->GetIndividualHP();
			}
		}
	}
	*/
}

void ACombat::SpreadDamageToBuildingPop(EAffiliation Receiver, int32& DamageLeft)
{
	if (DamageLeft <= 0) return;
	// get combat tiles with Receiver Building Pop
	TArray<FCombatTile*> BuildingPopCombatTiles;
	int32 TotalHP = 0;
	for (FCombatTile& CombatTile : CombatTiles)
	{
		if (CombatTile.Tile->GetBuilding()
			&& CombatTile.Tile->GetClaimant()
			&& CombatTile.Tile->GetClaimant()->Affiliation == Receiver
			&& CombatTile.Tile->GetBuilding()->Population->GetSize() > 0)
		{
			/*
			BuildingPopCombatTiles.Add(&CombatTile);
			UCombatValues* CV = CombatTile.Tile->Building->GetCombatValues();
			TotalHP += CV->GetHP();
			*/
		}
	}
	if (TotalHP <= 0) return;
	// entities
	int32 Damage = DamageLeft;
	// spread damage based on total hp ratio
	/*
	for (FCombatTile* BuildingPopCombatTile : BuildingPopCombatTiles)
	{
		UCombatValues* CV = BuildingPopCombatTile->Tile->Building->GetCombatValues();
		int32 BuildingPopDamage = CV->GetHP() / TotalHP * Damage;
		int32 BuildingPopKills = FMath::Min(BuildingPopDamage / CV->GetIndividualHP(), CV->GetIndividuals());
		BuildingPopCombatTile->BuildingPopKills = BuildingPopKills;
		DamageLeft -= BuildingPopKills * CV->GetIndividualHP();
	}
	// spread left over damage
	BuildingPopCombatTiles.Sort([&](const FCombatTile& A, const FCombatTile& B)
	{
		return A.Tile->Building->GetCombatValues()->GetIndividualHP()
			> B.Tile->Building->GetCombatValues()->GetIndividualHP();
	});
	bool HasKilledSomething = true;
	while (HasKilledSomething)
	{
		HasKilledSomething = false;
		for (FCombatTile* BuildingPopCombatTile : BuildingPopCombatTiles)
		{
			UCombatValues* CV = BuildingPopCombatTile->Tile->Building->GetCombatValues();
			if (CV->GetIndividuals() > BuildingPopCombatTile->BuildingPopKills
				&& DamageLeft >= CV->GetIndividualHP())
			{
				HasKilledSomething = true;
				++BuildingPopCombatTile->BuildingPopKills;
				DamageLeft -= CV->GetIndividualHP();
			}
		}
	}
	*/
}

void ACombat::SpreadDamageToBuildings(EAffiliation Receiver, int32& DamageLeft)
{
	/*
	if (DamageLeft <= 0) return;
	int32 BuildingTierHP = GameBalance->BuildingTierHP;
	// get combat tiles with Receiver Building Pop
	TArray<FCombatTile*> BuildingCombatTiles;
	int32 TotalHP = 0;
	for (FCombatTile& CombatTile : CombatTiles)
	{
		if (CombatTile.Tile->Building
			&& CombatTile.Tile->GetClaimant()
			&& CombatTile.Tile->GetClaimant()->Affiliation == Receiver)
		{
			BuildingCombatTiles.Add(&CombatTile);
			TotalHP += BuildingTierHP * CombatTile.Tile->Building->Tier;
		}
	}
	if (TotalHP <= 0) return;
	// entities
	int32 Damage = DamageLeft;
	// spread damage based on total hp ratio
	for (FCombatTile* BuildingCombatTile : BuildingCombatTiles)
	{
		int32 HP = BuildingTierHP * BuildingCombatTile->Tile->Building->Tier;
		int32 BuildingDamage = HP / TotalHP * Damage;
		int32 BuildingKills = FMath::Min(BuildingDamage / BuildingTierHP, BuildingCombatTile->Tile->Building->Tier);
		BuildingCombatTile->BuildingDowngrade = BuildingKills;
		DamageLeft -= BuildingKills * BuildingTierHP;
	}
	// spread left over damage
	bool HasKilledSomething = true;
	while (HasKilledSomething)
	{
		HasKilledSomething = false;
		for (FCombatTile* BuildingCombatTile : BuildingCombatTiles)
		{
			if (BuildingCombatTile->Tile->Building->Tier > BuildingCombatTile->BuildingDowngrade
				&& DamageLeft >= BuildingTierHP)
			{
				HasKilledSomething = true;
				++BuildingCombatTile->BuildingDowngrade;
				DamageLeft -= BuildingTierHP;
			}
		}
	}
	*/
}

void ACombat::CountKills()
{
	AlliedPopKills = 0;
	AlliedBuildingKills = 0;
	EnemyPopKills = 0;
	EnemyBuildingKills = 0;
	for (const FCombatTile& CombatTile : CombatTiles)
	{
		AlliedPopKills += CombatTile.AlliedEntityKills;
		EnemyPopKills += CombatTile.EnemyEntityKills;
		if (CombatTile.Tile->GetBuilding() && CombatTile.Tile->GetClaimant())
		{
			if (CombatTile.Tile->GetClaimant()->Affiliation == EAffiliation::Ally)
			{
				AlliedBuildingKills += CombatTile.BuildingDowngrade;
				AlliedPopKills += CombatTile.BuildingPopKills;
			}
			else
			{
				EnemyBuildingKills += CombatTile.BuildingDowngrade;
				EnemyPopKills += CombatTile.BuildingPopKills;
			}
		}
	}
}

void ACombat::TriggerCombat()
{
	/*
	PreventCalcKills = true;
	for (FCombatTile CombatTile : CombatTiles)
	{
		ATile* Tile = CombatTile.Tile;
		if (Tile->GetAlliedEntity())
		{
			Tile->GetAlliedEntity()->KillIndividuals(CombatTile.AlliedEntityKills);
		}
		if (Tile->GetEnemyEntity())
		{
			Tile->GetEnemyEntity()->KillIndividuals(CombatTile.EnemyEntityKills);
		}
		if (Tile->GetBuilding())
		{
			Tile->GetBuilding()->Population->DecreaseSize(CombatTile.BuildingPopKills);
			if (CombatTile.BuildingDowngrade > 0) Tile->Unbuild(); // TODO: Downgrade instead
		}
	}
	PreventCalcKills = false;
	*/
}

void ACombat::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	for (int32 i = CombatTiles.Num() - 1; i >= 0; --i)
	{
		RemoveCombatTile(CombatTiles[i]);
	}
}

void ACombat::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	MainMesh->AddRelativeRotation(FRotator(0, DeltaSeconds * 6, 0));
}

int32 ACombat::GetAlliedAttack() const
{
	return AlliedAttack;
}

int32 ACombat::GetAlliedDefense() const
{
	return AlliedDefense;
}

int32 ACombat::GetEnemyAttack() const
{
	return EnemyAttack;
}

int32 ACombat::GetEnemyDefense() const
{
	return EnemyDefense;
}

int32 ACombat::GetAlliedDamage() const
{
	return AlliedDamage;
}

int32 ACombat::GetEnemyDamage() const
{
	return EnemyDamage;
}

int32 ACombat::GetAlliedPopKills() const
{
	return AlliedPopKills;
}

int32 ACombat::GetAlliedBuildingKills() const
{
	return AlliedBuildingKills;
}

int32 ACombat::GetEnemyPopKills() const
{
	return EnemyPopKills;
}

int32 ACombat::GetEnemyBuildingKills() const
{
	return EnemyBuildingKills;
}
