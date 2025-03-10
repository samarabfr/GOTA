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

	DOREPLIFETIME_WITH_PARAMS(UEcoValues, MaxTrees, Params);
	DOREPLIFETIME_WITH_PARAMS(UEcoValues, MaxForage, Params);

	DOREPLIFETIME_WITH_PARAMS(UEcoValues, TreeGrowthProgress, Params);
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
		TEXT("/Game/Tile/DA_EcoSystem"));
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
	
	if (Forage == MaxForage)
		ForageGrowthProgress = 0;
	else
		ForageGrowthProgress += ForageGrowth * DeltaSeconds;
}

void UEcoValues::SetMaxValues(int32 NewMaxTrees, int32 NewMaxForage)
{
	// Set Max Trees
	MaxTrees = NewMaxTrees;
	if (MaxTrees < 0) MaxTrees = 0;
	if (Trees > MaxTrees) SubtractTrees(Trees - MaxTrees);
	MARK_PROPERTY_DIRTY_FROM_NAME(UEcoValues, MaxTrees, this)
	
	// Set Max Forage
	MaxForage = NewMaxForage;
	if (MaxForage < 0) MaxForage = 0;
	if (Forage > MaxForage) SubtractForage(Forage - MaxForage);
	MARK_PROPERTY_DIRTY_FROM_NAME(UEcoValues, MaxForage, this)
}

void UEcoValues::MaxALlValues()
{
	SetTrees(MaxTrees);
	SetForage(MaxForage);
}

void UEcoValues::NeighborChangedTrees(int32 Amount)
{
	TreeGrowth += Amount * DA_EcoSystem->TreeGrowthPerNeighborTree;
}

void UEcoValues::NeighborChangedForage(int32 Amount)
{
	ForageGrowth += Amount * DA_EcoSystem->ForageGrowthPerNeighborForage;
}

// -------------------OnChange-------------------------

void UEcoValues::OnTreeChanges(int32 Change)
{
	TreeGrowth += Change * DA_EcoSystem->TreeGrowthPerOwnTree;
	ForageGrowth += Change * DA_EcoSystem->ForageGrowthPerOwnTree;
	OnTreesChanged.Broadcast(Change);
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

void UEcoValues::SubtractForage(int32 Amount)
{
	int32 OldValue = Forage;
	Forage -= Amount;
	if (Forage < 0) Forage = 0;
	if (Forage == OldValue) return;
	OnForageChanges(Forage - OldValue);
	MARK_PROPERTY_DIRTY_FROM_NAME(UEcoValues, Forage, this)
}
