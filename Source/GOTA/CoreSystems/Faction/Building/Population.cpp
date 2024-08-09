#include "Population.h"

FPopulation FPopulation::operator+(const FPopulation& Other) const
{
	FPopulation Result;

	Result.Size = this->Size + Other.Size;
	Result.MaxSize = this->MaxSize + Other.MaxSize;
	Result.FollowerColonists = this->FollowerColonists + Other.FollowerColonists;
	Result.FollowerGuardian1 = this->FollowerGuardian1 + Other.FollowerGuardian1;
	Result.FollowerGuardian2 = this->FollowerGuardian2 + Other.FollowerGuardian2;
	Result.FollowerGuardian3 = this->FollowerGuardian3 + Other.FollowerGuardian3;
	Result.FollowerGuardian4 = this->FollowerGuardian4 + Other.FollowerGuardian4;
	Result.MoodAngry = this->MoodAngry + Other.MoodAngry;
	Result.MoodFear = this->MoodFear + Other.MoodFear;

	return Result;
}

FPopulation FPopulation::operator+=(const FPopulation& Other)
{
	this->Size += Other.Size;
	this->MaxSize += Other.MaxSize;
	this->FollowerColonists += Other.FollowerColonists;
	this->FollowerGuardian1 += Other.FollowerGuardian1;
	this->FollowerGuardian2 += Other.FollowerGuardian2;
	this->FollowerGuardian3 += Other.FollowerGuardian3;
	this->FollowerGuardian4 += Other.FollowerGuardian4;
	this->MoodAngry += Other.MoodAngry;
	this->MoodFear += Other.MoodFear;

	return *this;
}

FPopulation FPopulation::operator-(const FPopulation& Other) const
{
	FPopulation Result;

	Result.Size = this->Size - Other.Size;
	Result.MaxSize = this->MaxSize - Other.MaxSize;
	Result.FollowerColonists = this->FollowerColonists - Other.FollowerColonists;
	Result.FollowerGuardian1 = this->FollowerGuardian1 - Other.FollowerGuardian1;
	Result.FollowerGuardian2 = this->FollowerGuardian2 - Other.FollowerGuardian2;
	Result.FollowerGuardian3 = this->FollowerGuardian3 - Other.FollowerGuardian3;
	Result.FollowerGuardian4 = this->FollowerGuardian4 - Other.FollowerGuardian4;
	Result.MoodAngry = this->MoodAngry - Other.MoodAngry;
	Result.MoodFear = this->MoodFear - Other.MoodFear;

	return Result;
}

FPopulation FPopulation::operator-=(const FPopulation& Other)
{
	this->Size -= Other.Size;
	this->MaxSize -= Other.MaxSize;
	this->FollowerColonists -= Other.FollowerColonists;
	this->FollowerGuardian1 -= Other.FollowerGuardian1;
	this->FollowerGuardian2 -= Other.FollowerGuardian2;
	this->FollowerGuardian3 -= Other.FollowerGuardian3;
	this->FollowerGuardian4 -= Other.FollowerGuardian4;
	this->MoodAngry -= Other.MoodAngry;
	this->MoodFear -= Other.MoodFear;

	return *this;
}
