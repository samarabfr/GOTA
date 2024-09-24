#include "GameResources.h"

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