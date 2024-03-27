#include "HexCoords.h"

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