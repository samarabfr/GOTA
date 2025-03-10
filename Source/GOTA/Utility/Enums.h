#pragma once

UENUM()
enum class EAffiliation : uint8
{
	Ally UMETA(DisplayName = "Ally"),
	Enemy UMETA(DisplayName = "Enemy")
};

inline EAffiliation operator!(EAffiliation Affiliation)
{
	if (Affiliation == EAffiliation::Ally)
		return EAffiliation::Enemy;
	return EAffiliation::Ally;
}

UENUM()
enum class EFaction : uint8
{
	None UMETA(DisplayName = "None"),
	Colonists UMETA(DisplayName = "Colonists"),
	Natives UMETA(DisplayName = "Natives"),
	Guardians UMETA(DisplayName = "Guardians"),
	Enum_Length UMETA(Hidden)
};

UENUM()
enum class EProductionType : uint8
{
	None UMETA(DisplayName = "None"),
	Food UMETA(DisplayName = "Food"),
	Wood UMETA(DisplayName = "Wood"),
	Stone UMETA(DisplayName = "Stone"),
	Construction UMETA(DisplayName = "Construction"),
	Healing UMETA(DisplayName = "Healing"),
	Enum_Length UMETA(Hidden)
};

UENUM()
enum class EMood : uint8
{
	Content UMETA(DisplayName = "Content"),
	Angry UMETA(DisplayName = "Angry"),
	Fear UMETA(DisplayName = "Fear"),
	Enum_Length UMETA(Hidden)
};

UENUM()
enum class EBiome : uint8
{
	Gras UMETA(DisplayName = "Gras"),
	Beach UMETA(DisplayName = "Beach"),
	Mountain UMETA(DisplayName = "Mountain"),
	Volcano UMETA(DisplayName = "Volcano")
};

UENUM()
enum class EEcoValue : uint8
{
	Tree UMETA(DisplayName = "Tree"),
	Forage UMETA(DisplayName = "Forage")
};

UENUM()
enum class EGameEnding : uint8
{
	NativesWon UMETA(DisplayName = "NativesWon"),
	ColonistsWon UMETA(DisplayName = "ColonistsWon")
};

UENUM()
enum class EGameStatus : uint8
{
	Lobby UMETA(DisplayName = "Lobby"),
	Loading UMETA(DisplayName = "Loading"),
	Running UMETA(DisplayName = "Running"),
	Ended UMETA(DisplayName = "Ended")
};

UENUM()
enum class ECivilianStatus : uint8
{
	Idling UMETA(DisplayName = "Idling"),
	Moving UMETA(DisplayName = "Moving to next tile"),
	Working UMETA(DisplayName = "Working")
};

UENUM()
enum class EArmyStatus : uint8
{
	Idling UMETA(DisplayName = "Idling"),
	MovingToNextTile UMETA(DisplayName = "Moving to next tile"),
	RecruitingFromTile UMETA(DisplayName = "Recruiting from tile"),
	Attacking UMETA(DisplayName = "Fighting"),
	Ravaging UMETA(DisplayName = "Ravaging")
};

UENUM()
enum class EArmyMode : uint8
{
	GarrisonMode UMETA(DisplayName = "Garrison mode"),
	AttackMode UMETA(DisplayName = "Attack mode"),
	GuardMode UMETA(DisplayName = "Guard mode"),
	InterceptMode UMETA(DisplayName = "Intercept mode")
};

UENUM()
enum class EEntityType : uint8
{
	None UMETA(DisplayName = "None"),
	Civilian UMETA(DisplayName = "Civilian"),
	Army UMETA(DisplayName = "Military"),
	Other UMETA(DisplayName = "Other")
};

UENUM()
enum class EAbilityCategory : uint8
{
	Debug UMETA(DisplayName = "Debug"),
	Common UMETA(DisplayName = "Common"),
	Fire UMETA(DisplayName = "Fire")
};

UENUM()
enum class EResource : uint8
{
	None UMETA(DisplayName = "None"),
	Food UMETA(DisplayName = "Food"),
	Wood UMETA(DisplayName = "Wood"),
	Stone UMETA(DisplayName = "Stone"),
	Forage UMETA(DisplayName = "Forage"),
	Trees UMETA(DisplayName = "Trees"),
	Enum_Length UMETA(Hidden)
};


