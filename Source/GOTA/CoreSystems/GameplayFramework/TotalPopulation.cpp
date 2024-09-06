#include "TotalPopulation.h"

void UTotalPopulation::RegisterPop(UPopulation* Pop)
{
	Pop->OnSizeChanged.AddDynamic(this, &UTotalPopulation::UpdateSize);
	UpdateSize(Pop->GetSize());
	Pop->OnAngryChanged.AddDynamic(this, &UTotalPopulation::UpdateAngry);
	UpdateAngry(Pop->GetAngry());
	Pop->OnFearChanged.AddDynamic(this, &UTotalPopulation::UpdateFear);
	UpdateFear(Pop->GetFear());
}

void UTotalPopulation::UnregisterPop(UPopulation* Pop)
{
	Pop->OnSizeChanged.RemoveDynamic(this, &UTotalPopulation::UpdateSize);
	UpdateSize(-Pop->GetSize());
	Pop->OnAngryChanged.RemoveDynamic(this, &UTotalPopulation::UpdateAngry);
	UpdateAngry(-Pop->GetAngry());
	Pop->OnFearChanged.RemoveDynamic(this, &UTotalPopulation::UpdateFear);
	UpdateFear(-Pop->GetFear());
}

void UTotalPopulation::UpdateSize(int16 ChangedBy)
{
	Size += ChangedBy;
}

void UTotalPopulation::UpdateAngry(int16 ChangedBy)
{
	Angry += ChangedBy;
}

void UTotalPopulation::UpdateFear(int16 ChangedBy)
{
	Fear += ChangedBy;
}
