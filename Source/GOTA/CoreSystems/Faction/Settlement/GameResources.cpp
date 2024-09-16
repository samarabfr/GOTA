#include "GameResources.h"

FGameResources& FGameResources::operator+=(const FGameResources& Addend)
{
	this->Food += Addend.Food;
	this->Wood += Addend.Wood;
	this->Stone += Addend.Stone;
	return *this; 
}