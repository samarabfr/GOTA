#include "ConstructionResources.h"

void FConstructionResources::Add(const float Amount, const EProductionType ProductionType)
{
	switch (ProductionType)
	{
	case EProductionType::Food:
		Food += Amount;
		break;

	case EProductionType::Wood:
		Wood += Amount;
		break;

	case EProductionType::Stone:
		Stone += Amount;
		break;

	default:
		break;
	}
}

void FConstructionResources::Remove(const float Amount, const EProductionType ProductionType)
{
	switch (ProductionType)
	{
	case EProductionType::Food:
		Food -= Amount;
		break;

	case EProductionType::Wood:
		Wood -= Amount;
		break;

	case EProductionType::Stone:
		Stone -= Amount;
		break;

	default:
		break;
	}
}

void FConstructionResources::Add(const float Amount, const EResource Resource)
{
	switch (Resource)
	{
	case EResource::Food:
		Food += Amount;
		break;

	case EResource::Wood:
		Wood += Amount;
		break;

	case EResource::Stone:
		Stone += Amount;
		break;

	default:
		break;
	}
}

void FConstructionResources::Remove(const float Amount, const EResource Resource)
{
	switch (Resource)
	{
	case EResource::Food:
		Food -= Amount;
		break;

	case EResource::Wood:
		Wood -= Amount;
		break;

	case EResource::Stone:
		Stone -= Amount;
		break;

	default:
		break;
	}
}

FConstructionResources& FConstructionResources::operator+=(const FConstructionResources& Other)
{
	this->Food += Other.Food;
	this->Wood += Other.Wood;
	this->Stone += Other.Stone;
	return *this;
}

FConstructionResources& FConstructionResources::operator-=(const FConstructionResources& Other)
{
	this->Food -= Other.Food;
	this->Wood -= Other.Wood;
	this->Stone -= Other.Stone;
	return *this;
}

bool FConstructionResources::operator<(const FConstructionResources& Other) const
{
	return this->Food < Other.Food && this->Wood < Other.Wood && this->Stone < Other.Stone;
}

bool FConstructionResources::operator>(const FConstructionResources& Other) const
{
	return this->Food > Other.Food && this->Wood > Other.Wood && this->Stone > Other.Stone;
}

bool FConstructionResources::operator<=(const FConstructionResources& Other) const
{
	return this->Food <= Other.Food && this->Wood <= Other.Wood && this->Stone <= Other.Stone;
}

bool FConstructionResources::operator>=(const FConstructionResources& Other) const
{
	return this->Food >= Other.Food && this->Wood >= Other.Wood && this->Stone >= Other.Stone;
}

bool FConstructionResources::operator==(const FConstructionResources& Other) const
{
	return this->Food == Other.Food && this->Wood == Other.Wood && this->Stone == Other.Stone;
}

bool FConstructionResources::operator!=(const FConstructionResources& Other) const
{
	return this->Food != Other.Food || this->Wood != Other.Wood || this->Stone != Other.Stone;
}

FConstructionResources FConstructionResources::operator+(const FConstructionResources& Other) const
{
	FConstructionResources Result;
	Result.Food = this->Food + Other.Food;
	Result.Wood = this->Wood + Other.Wood;
	Result.Stone = this->Stone + Other.Stone;
	return Result;
}

FConstructionResources FConstructionResources::operator-(const FConstructionResources& Other) const
{
	FConstructionResources Result;
	Result.Food = this->Food - Other.Food;
	Result.Wood = this->Wood - Other.Wood;
	Result.Stone = this->Stone - Other.Stone;
	return Result;
}
