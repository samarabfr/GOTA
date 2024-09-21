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