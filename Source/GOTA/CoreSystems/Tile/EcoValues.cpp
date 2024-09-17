#include "EcoValues.h"
#include "Net/UnrealNetwork.h"
#include "Net/Core/PushModel/PushModel.h"

void UEcoValues::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	UObject::GetLifetimeReplicatedProps(OutLifetimeProps);
	FDoRepLifetimeParams Params;
	Params.bIsPushBased = true;
	Params.Condition = COND_None;
	Params.RepNotifyCondition = REPNOTIFY_OnChanged;
	DOREPLIFETIME_WITH_PARAMS(UEcoValues, Trees, Params);
	DOREPLIFETIME_WITH_PARAMS(UEcoValues, Forage, Params);
	DOREPLIFETIME_WITH_PARAMS(UEcoValues, Wildlife, Params);

	DOREPLIFETIME_WITH_PARAMS(UEcoValues, MaxTrees, Params);
	DOREPLIFETIME_WITH_PARAMS(UEcoValues, MaxWildlife, Params);
	DOREPLIFETIME_WITH_PARAMS(UEcoValues, MaxForage, Params);

	DOREPLIFETIME_WITH_PARAMS(UEcoValues, TreeGrowthProgress, Params);
	DOREPLIFETIME_WITH_PARAMS(UEcoValues, WildlifeGrowthProgress, Params);
	DOREPLIFETIME_WITH_PARAMS(UEcoValues, ForageGrowthProgress, Params);
}

bool UEcoValues::IsSupportedForNetworking() const
{
	return true;
}

UEcoValues::UEcoValues()
{
	// Load DA_EcoSystem
	static ConstructorHelpers::FObjectFinder<UEcoSystemDataAsset> DataAsset(
		TEXT("/Game/CoreSystems/Tile/DA_EcoSystem"));
	if (DataAsset.Succeeded())
	{
		DA_EcoSystem = DataAsset.Object;
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("EcoValues couldn't load EcoSystem Data Asset"))
	}
}

void UEcoValues::Init(EBiome Biome)
{
	if (DA_EcoSystem->BaseTreeGrowthPerBiome.Contains(Biome))
	{
		TreeGrowth += *DA_EcoSystem->BaseTreeGrowthPerBiome.Find(Biome);
	}
	if (DA_EcoSystem->BaseWildlifeGrowthPerBiome.Contains(Biome))
	{
		WildlifeGrowth += *DA_EcoSystem->BaseWildlifeGrowthPerBiome.Find(Biome);
	}
	if (DA_EcoSystem->BaseForageGrowthPerBiome.Contains(Biome))
	{
		ForageGrowth += *DA_EcoSystem->BaseForageGrowthPerBiome.Find(Biome);
	}
}

void UEcoValues::ServerTick(double DeltaSeconds)
{
	ClientTick(DeltaSeconds);

	if (TreeGrowthProgress >= DA_EcoSystem->TreeGrowthThreshold)
	{
		int32 Count = TreeGrowthProgress / DA_EcoSystem->TreeGrowthThreshold;
		AddTrees(Count);
		TreeGrowthProgress -= Count * DA_EcoSystem->TreeGrowthThreshold;
		MARK_PROPERTY_DIRTY_FROM_NAME(UEcoValues, TreeGrowthProgress, this)
	}
	if (WildlifeGrowthProgress >= DA_EcoSystem->WildlifeGrowthThreshold)
	{
		int32 Count = WildlifeGrowthProgress / DA_EcoSystem->WildlifeGrowthThreshold;
		AddWildlife(Count);
		WildlifeGrowthProgress -= Count * DA_EcoSystem->WildlifeGrowthThreshold;
		MARK_PROPERTY_DIRTY_FROM_NAME(UEcoValues, WildlifeGrowthProgress, this)
	}
	if (ForageGrowthProgress >= DA_EcoSystem->ForageGrowthThreshold)
	{
		int32 Count = ForageGrowthProgress / DA_EcoSystem->ForageGrowthThreshold;
		AddForage(Count);
		ForageGrowthProgress -= Count * DA_EcoSystem->ForageGrowthThreshold;
		MARK_PROPERTY_DIRTY_FROM_NAME(UEcoValues, ForageGrowthProgress, this)
	}
}

void UEcoValues::ClientTick(double DeltaSeconds)
{
	if (Trees == MaxTrees)
		TreeGrowthProgress = 0;
	else
		TreeGrowthProgress += TreeGrowth * DeltaSeconds;

	if (Wildlife == MaxWildlife)
		WildlifeGrowthProgress = 0;
	else
		WildlifeGrowthProgress += WildlifeGrowth * DeltaSeconds;

	if (Forage == MaxForage)
		ForageGrowthProgress = 0;
	else
		ForageGrowthProgress += ForageGrowth * DeltaSeconds;
}

void UEcoValues::SetMaxValues(int32 NewMaxTrees, EBiome Biome)
{
	// Set Max Trees
	MaxTrees = NewMaxTrees;
	if (MaxTrees < 0) MaxTrees = 0;
	if (Trees > MaxTrees) SubtractTrees(Trees - MaxTrees);
	MARK_PROPERTY_DIRTY_FROM_NAME(UEcoValues, MaxTrees, this)
	// Set Max Forage
	MaxForage = 0;
	if (DA_EcoSystem->MaxForagePerBiome.Contains(Biome))
	{
		MaxForage += *DA_EcoSystem->MaxForagePerBiome.Find(Biome);
	}
	MaxForage += MaxTrees * DA_EcoSystem->MaxForagePerMaxTree;
	MARK_PROPERTY_DIRTY_FROM_NAME(UEcoValues, MaxForage, this)
	// Set Max Wildlife
	MaxWildlife = MaxForage * DA_EcoSystem->MaxWildlifePerForage;
	MARK_PROPERTY_DIRTY_FROM_NAME(UEcoValues, MaxWildlife, this)
}

void UEcoValues::MaxALlValues()
{
	SetTrees(MaxTrees);
	SetWildlife(MaxWildlife);
	SetForage(MaxForage);
}

void UEcoValues::AddNeighborTrees(int32 Amount)
{
	TreeGrowth += Amount * DA_EcoSystem->TreeGrowthPerNeighborTree;
}

void UEcoValues::AddNeighborWildlife(int32 Amount)
{
	WildlifeGrowth += Amount * DA_EcoSystem->WildlifeGrowthPerNeighborWildlife;
}

void UEcoValues::AddNeighborForage(int32 Amount)
{
	ForageGrowth += Amount * DA_EcoSystem->ForageGrowthPerNeighborForage;
}

void UEcoValues::SubtractNeighborTrees(int32 Amount)
{
	TreeGrowth -= Amount * DA_EcoSystem->TreeGrowthPerNeighborTree;
}

void UEcoValues::SubtractNeighborWildlife(int32 Amount)
{
	WildlifeGrowth -= Amount * DA_EcoSystem->WildlifeGrowthPerNeighborWildlife;
}

void UEcoValues::SubtractNeighborForage(int32 Amount)
{
	ForageGrowth -= Amount * DA_EcoSystem->ForageGrowthPerNeighborForage;
}

// -------------------OnChange-------------------------

void UEcoValues::OnTreeChanges(int32 Change)
{
	TreeGrowth += Change * DA_EcoSystem->TreeGrowthPerOwnTree;
	ForageGrowth += Change * DA_EcoSystem->ForageGrowthPerOwnTree;
	OnTreesChanged.Broadcast(Change);
}

void UEcoValues::OnWildlifeChanges(int32 Change)
{
	WildlifeGrowth += Change * DA_EcoSystem->WildlifeGrowthPerOwnWildlife;
	OnWildlifeChanged.Broadcast(Change);
}

void UEcoValues::OnForageChanges(int32 Change)
{
	ForageGrowth += Change * DA_EcoSystem->ForageGrowthPerOwnForage;
	OnForageChanged.Broadcast(Change);
}

// -------------------Setter-------------------------

void UEcoValues::SetTrees(int32 NewTrees)
{
	if (NewTrees > Trees)
		AddTrees(NewTrees - Trees);
	else
		SubtractTrees(Trees - NewTrees);
}

void UEcoValues::SetWildlife(int32 NewWildlife)
{
	if (NewWildlife > Wildlife)
		AddWildlife(NewWildlife - Wildlife);
	else
		SubtractWildlife(Wildlife - NewWildlife);
}

void UEcoValues::SetForage(int32 NewForage)
{
	if (NewForage > Forage)
		AddForage(NewForage - Forage);
	else
		SubtractForage(Forage - NewForage);
}

// -------------------OnReps-------------------------

void UEcoValues::OnRep_Trees(int32 OldValue)
{
	OnTreeChanges(Trees - OldValue);
}

void UEcoValues::OnRep_Wildlife(int32 OldValue)
{
	OnWildlifeChanges(Wildlife - OldValue);
}

void UEcoValues::OnRep_Forage(int32 OldValue)
{
	OnForageChanges(Forage - OldValue);
}

// -------------------Adder-------------------------

void UEcoValues::AddTrees(int32 Amount)
{
	int32 OldValue = Trees;
	Trees += Amount;
	if (Trees > MaxTrees) Trees = MaxTrees;
	if (Trees == OldValue) return;
	OnTreeChanges(Trees - OldValue);
	MARK_PROPERTY_DIRTY_FROM_NAME(UEcoValues, Trees, this)
}

void UEcoValues::AddWildlife(int32 Amount)
{
	int32 OldValue = Wildlife;
	Wildlife += Amount;
	if (Wildlife > MaxWildlife) Wildlife = MaxWildlife;
	if (Wildlife == OldValue) return;
	OnWildlifeChanges(Wildlife - OldValue);
	MARK_PROPERTY_DIRTY_FROM_NAME(UEcoValues, Wildlife, this)
}

void UEcoValues::AddForage(int32 Amount)
{
	int32 OldValue = Forage;
	Forage += Amount;
	if (Forage > MaxForage) Forage = MaxForage;
	if (Forage == OldValue) return;
	OnForageChanges(Forage - OldValue);
	MARK_PROPERTY_DIRTY_FROM_NAME(UEcoValues, Forage, this)
}

// -------------------Subtract-er-------------------------

void UEcoValues::SubtractTrees(int32 Amount)
{
	int32 OldValue = Trees;
	Trees -= Amount;
	if (Trees < 0) Trees = 0;
	if (Trees == OldValue) return;
	OnTreeChanges(Trees - OldValue);
	MARK_PROPERTY_DIRTY_FROM_NAME(UEcoValues, Trees, this)
}

void UEcoValues::SubtractWildlife(int32 Amount)
{
	int32 OldValue = Wildlife;
	Wildlife -= Amount;
	if (Wildlife < 0) Wildlife = 0;
	if (Wildlife == OldValue) return;
	OnWildlifeChanges(Wildlife - OldValue);
	MARK_PROPERTY_DIRTY_FROM_NAME(UEcoValues, Wildlife, this)
}

void UEcoValues::SubtractForage(int32 Amount)
{
	int32 OldValue = Forage;
	Forage -= Amount;
	if (Forage < 0) Forage = 0;
	if (Forage == OldValue) return;
	OnForageChanges(Forage - OldValue);
	MARK_PROPERTY_DIRTY_FROM_NAME(UEcoValues, Forage, this)
}
