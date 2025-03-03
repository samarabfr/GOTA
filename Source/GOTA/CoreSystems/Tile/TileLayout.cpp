#include "TileLayout.h"

FTileLayout::FTileLayout()
{
	Layout = ETileLayout::Layout_01;
	HexagonMesh = nullptr;
	HasRiver = false;
	RiverConnections.SetNum(6);
}

int32 FTileLayout::GetValidRotation(const TArray<bool>& InRiverConnections) const
{
	// invalid Connection Array
	if (InRiverConnections.Num() != 6) return -1;

	TArray<int32> ValidRotations;

	for (int32 Rotation = 0; Rotation < 6; ++Rotation)
	{
		bool ThisRotationWorks = true;
		for (int i = 0; i < 6; ++i)
		{
			if (RiverConnections[i] != InRiverConnections[(i + Rotation) % 6])
			{
				ThisRotationWorks = false;
				break;
			}
		}
		if (ThisRotationWorks)
		{
			ValidRotations.Add(Rotation);
		}
	}

	if (ValidRotations.Num() == 0)
	{
		return -1;
	}
	return ValidRotations[FMath::RandRange(0, ValidRotations.Num() - 1)];
}
