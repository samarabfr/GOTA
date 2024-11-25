#include "GameResources.h"

void FGameResources::AddProduction(const float Amount, const EProductionType ProductionType)
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

void FGameResources::RemoveProduction(const float Amount, const EProductionType ProductionType)
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

void FGameResources::AddConsumption(const float Amount, const EConsumptionType ConsumptionType)
{
	switch (ConsumptionType)
	{
	case EConsumptionType::Food:
		Food += Amount;
		break;

	case EConsumptionType::Wood:
		Wood += Amount;
		break;

	case EConsumptionType::Stone:
		Stone += Amount;
		break;

	default:
		break;
	}
}

void FGameResources::RemoveConsumption(const float Amount, const EConsumptionType ConsumptionType)
{
	switch (ConsumptionType)
	{
	case EConsumptionType::Food:
		Food -= Amount;
		break;

	case EConsumptionType::Wood:
		Wood -= Amount;
		break;

	case EConsumptionType::Stone:
		Stone -= Amount;
		break;

	default:
		break;
	}
}

FGameResources& FGameResources::operator+=(const FGameResources& Other)
{
	this->Food += Other.Food;
	this->Wood += Other.Wood;
	this->Stone += Other.Stone;
	return *this;
}

FGameResources& FGameResources::operator-=(const FGameResources& Other)
{
	this->Food -= Other.Food;
	this->Wood -= Other.Wood;
	this->Stone -= Other.Stone;
	return *this;
}

bool FGameResources::operator<(const FGameResources& Other) const
{
	return this->Food < Other.Food && this->Wood < Other.Wood && this->Stone < Other.Stone;
}

bool FGameResources::operator>(const FGameResources& Other) const
{
	return this->Food > Other.Food && this->Wood > Other.Wood && this->Stone > Other.Stone;
}

bool FGameResources::operator<=(const FGameResources& Other) const
{
	return this->Food <= Other.Food && this->Wood <= Other.Wood && this->Stone <= Other.Stone;
}

bool FGameResources::operator>=(const FGameResources& Other) const
{
	return this->Food >= Other.Food && this->Wood >= Other.Wood && this->Stone >= Other.Stone;
}

FGameResources FGameResources::operator+(const FGameResources& Other) const
{
	FGameResources Result;
	Result.Food = this->Food + Other.Food;
	Result.Wood = this->Wood + Other.Wood;
	Result.Stone = this->Stone + Other.Stone;
	return Result;
}

FGameResources FGameResources::operator-(const FGameResources& Other) const
{
	FGameResources Result;
	Result.Food = this->Food - Other.Food;
	Result.Wood = this->Wood - Other.Wood;
	Result.Stone = this->Stone - Other.Stone;
	return Result;
}
