#include "HexCoords.h"

const float FHexCoords::Gridsize = 500.0f;

FHexCoords::FHexCoords()
{
	Q = 0;
	R = 0;
}

FHexCoords::FHexCoords(const int32 NewQ, const int32 NewR)
{
	Q = NewQ;
	R = NewR;
}

bool FHexCoords::operator==(const FHexCoords& Other) const
{
	return Equals(Other);
}

bool FHexCoords::Equals(const FHexCoords& Other) const
{
	return Q == Other.Q && R == Other.R;
}

FHexCoords FHexCoords::operator+(const FHexCoords& Other) const
{
	return FHexCoords(Q + Other.Q, R + Other.R);
}

FHexCoords FHexCoords::operator-(const FHexCoords& Other) const
{
	return FHexCoords(Q - Other.Q, R - Other.R);
}

int32 FHexCoords::DistanceTo(const FHexCoords Target) const
{
	const FHexCoords Diff = this & -Target;
	return (FMath::Abs(Diff.Q) +
			FMath::Abs(Diff.Q + Diff.R) +
			FMath::Abs(Diff.R))
		/ 2;
}
