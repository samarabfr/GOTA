// Fill out your copyright notice in the Description page of Project Settings.


#include "HexCoordsFunctions.h"
#include "TileMap.h"

FVector2D UHexCoordsFunctions::HexCoordsToVector2D(FHexCoords HexCoords)
{
	const double X = HexCoords.Q * 1.5;
	const double Y = HexCoords.Q * 0.866 + HexCoords.R  * 1.732;
	return FVector2D(X * FHexCoords::Gridsize, Y * FHexCoords::Gridsize);
}

FHexCoords UHexCoordsFunctions::Vector2DToHexCoords(FVector2D Vector)
{
	Vector = Vector / FHexCoords::Gridsize;
	const double FracQ = 0.667 * Vector.X;
	const double FracR = -0.333 * Vector.X + 0.577 * Vector.Y;
	const double FracS = -FracQ - FracR;
	const int32 RoundQ = round(FracQ);
	const int32 RoundR = round(FracR);
	const int32 RoundS = round(FracS);
	const int32 DiffQ = abs(RoundQ - FracQ);
	const int32 DiffR = abs(RoundR - FracR);
	const int32 DiffS = abs(RoundS - FracS);
	if (DiffQ > DiffR && DiffQ > DiffS)
	{
		return FHexCoords(-RoundR - RoundS , RoundR );
	}
	if (DiffR > DiffS)
	{
		return FHexCoords(RoundQ , -RoundQ - RoundS );
	}
	return FHexCoords(RoundQ, RoundR);
}

FHexCoords UHexCoordsFunctions::VectorToHexCoords(FVector Vector)
{
	return Vector2DToHexCoords(FVector2D(Vector.X, Vector.Y));
}

TArray<FHexCoords> UHexCoordsFunctions::GetAllCoordsInRange(FHexCoords Origin, int32 Range)
{
	TArray<FHexCoords> Coords;

	for (int32 Q = -Range; Q <= Range; ++Q)
	{
		for (int32 R = FMath::Max(-Range, -Q - Range); R <= FMath::Min(Range, -Q + Range); ++R)
		{
			int32 S = -Q - R;
			int32 newQ = Origin.Q + Q;
			int32 newR = Origin.R + R;
			Coords.Add(FHexCoords(newQ, newR));
		}
	}
	return Coords;
}
