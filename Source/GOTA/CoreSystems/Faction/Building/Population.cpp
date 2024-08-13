#include "Population.h"


void FPopulation::SetFollower(ECultureLoyalty Culture, int32 Value)
{
	switch (Culture)
	{
	case ECultureLoyalty::Colonists:
		FollowerColonists = Value;
		return;
	case ECultureLoyalty::Guardian1:
		FollowerGuardian1 = Value;
		return;
	case ECultureLoyalty::Guardian2:
		FollowerGuardian2 = Value;
		return;
	case ECultureLoyalty::Guardian3:
		FollowerGuardian3 = Value;
		return;
	case ECultureLoyalty::Guardian4:
		FollowerGuardian4 = Value;
	default: ;
	}
}

void FPopulation::SetMood(EMood Mood, int32 Value)
{
	switch (Mood)
	{
	case EMood::Content:
		MoodContent = Value;
		return;
	case EMood::Angry:
		MoodAngry = Value;
		return;
	case EMood::Fear:
		MoodFear = Value;
		return;
	default: ;
	}
}

int32 FPopulation::GetFollower(ECultureLoyalty Culture) const
{
	switch (Culture)
	{
	case ECultureLoyalty::Colonists:
		return FollowerColonists;
	case ECultureLoyalty::Guardian1:
		return FollowerGuardian1;
	case ECultureLoyalty::Guardian2:
		return FollowerGuardian2;
	case ECultureLoyalty::Guardian3:
		return FollowerGuardian3;
	case ECultureLoyalty::Guardian4:
		return FollowerGuardian4;
	default:
		return -1;
	}
}

int32 FPopulation::GetNativeFollowers() const
{
	return FollowerGuardian1 + FollowerGuardian2 + FollowerGuardian3 + FollowerGuardian4;
}


int32 FPopulation::GetMood(EMood Mood) const
{
	switch (Mood)
	{
	case EMood::Content:
		return MoodContent;
	case EMood::Angry:
		return MoodAngry;
	case EMood::Fear:
		return MoodFear;
	default:
		return -1;
	}
}

int32 FPopulation::SumFollower() const
{
	return FollowerColonists + FollowerGuardian1 + FollowerGuardian2 + FollowerGuardian3 + FollowerGuardian4;
}

int32 FPopulation::SumMood() const
{
	return MoodContent + MoodAngry + MoodFear;
}

bool FPopulation::AnyBiggerThan(const FPopulation& Other) const
{
	return this->Size > Other.Size
		|| this->MaxSize > Other.MaxSize
		|| this->FollowerColonists > Other.FollowerColonists
		|| this->FollowerGuardian1 > Other.FollowerGuardian1
		|| this->FollowerGuardian2 > Other.FollowerGuardian2
		|| this->FollowerGuardian3 > Other.FollowerGuardian3
		|| this->FollowerGuardian4 > Other.FollowerGuardian4
		|| this->MoodContent > Other.MoodContent
		|| this->MoodAngry > Other.MoodAngry
		|| this->MoodFear > Other.MoodFear
		|| this->Bows > Other.Bows
		|| this->Muskets > Other.Muskets
		|| this->Shields > Other.Shields;
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
	Result.Bows = this->Bows + Other.Bows;
	Result.Muskets = this->Muskets + Other.Muskets;
	Result.Shields = this->Shields + Other.Shields;
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
	this->Bows += Other.Bows;
	this->Muskets += Other.Muskets;
	this->Shields += Other.Shields;
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
	Result.Bows = this->Bows - Other.Bows;
	Result.Muskets = this->Muskets - Other.Muskets;
	Result.Shields = this->Shields - Other.Shields;
	return Result;
}

FPopulation FPopulation::operator-() const
{
	FPopulation Result;
	Result.Size = -this->Size;
	Result.MaxSize = -this->MaxSize;
	Result.FollowerColonists = -this->FollowerColonists;
	Result.FollowerGuardian1 = -this->FollowerGuardian1;
	Result.FollowerGuardian2 = -this->FollowerGuardian2;
	Result.FollowerGuardian3 = -this->FollowerGuardian3;
	Result.FollowerGuardian4 = -this->FollowerGuardian4;
	Result.MoodContent = -this->MoodContent;
	Result.MoodAngry = -this->MoodAngry;
	Result.MoodFear = -this->MoodFear;
	Result.Bows = -this->Bows;
	Result.Muskets = -this->Muskets;
	Result.Shields = -this->Shields;
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
	this->Bows -= Other.Bows;
	this->Muskets -= Other.Muskets;
	this->Shields -= Other.Shields;
	return *this;
}
