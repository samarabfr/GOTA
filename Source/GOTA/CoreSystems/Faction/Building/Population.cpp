#include "Population.h"

int32 FPopulation::GetNativeFollowers() const
{
	return FollowerGuardian1 + FollowerGuardian2 + FollowerGuardian3 + FollowerGuardian4;
}

void FPopulation::SetFollower(ECultureLoyalty Culture, int32 Value)
{
	switch (Culture)
	{
	case ECultureLoyalty::Colonists:
		FollowerColonists = Value;
	case ECultureLoyalty::Guardian1:
		FollowerGuardian1 = Value;
	case ECultureLoyalty::Guardian2:
		FollowerGuardian2 = Value;
	case ECultureLoyalty::Guardian3:
		FollowerGuardian3 = Value;
	case ECultureLoyalty::Guardian4:
		FollowerGuardian4 = Value;
	default: ;
	}
}

void FPopulation::SetMood(EMood Mood, int32 Value)
{
	switch (Mood) {
	case EMood::Content:
		MoodContent = Value;
		return;
	case EMood::Angry:
		MoodAngry = Value;
		return;
	case EMood::Fear:
		MoodFear = Value;
		return;
	default:;
	}
}

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
	Result.MoodContent = this->MoodContent + Other.MoodContent;
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
	this->MoodContent += Other.MoodContent;
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
	Result.MoodContent = this->MoodContent - Other.MoodContent;
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
	this->MoodContent -= Other.MoodContent;
	this->MoodAngry -= Other.MoodAngry;
	this->MoodFear -= Other.MoodFear;
	return *this;
}
