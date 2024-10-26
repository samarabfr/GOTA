#pragma once

UENUM(BlueprintType)
enum class EAffiliation : uint8
{
	Ally UMETA(DisplayName = "Ally"),
	Enemy UMETA(DisplayName = "Enemy")
};

inline EAffiliation operator!(EAffiliation Affiliation)
{
	if(Affiliation == EAffiliation::Ally)
		return EAffiliation::Enemy;
	return EAffiliation::Ally;
}

UENUM(BlueprintType)
enum class EFaction : uint8
{
	None UMETA(DisplayName = "None"),
	Colonists UMETA(DisplayName = "Colonists"),
	Natives UMETA(DisplayName = "Natives"),
	Guardians UMETA(DisplayName = "Guardians"),
	Enum_Length UMETA(Hidden)
};

UENUM(BlueprintType)
enum class EProductionType : uint8
{
	None UMETA(DisplayName = "None"),
	Food UMETA(DisplayName = "Food"),
	Wood UMETA(DisplayName = "Wood"),
	Stone UMETA(DisplayName = "Stone"),
	XPForGuardians UMETA(DisplayName = "XP For Guardians"),
	Enum_Length UMETA(Hidden)
};

UENUM(BlueprintType)
enum class EMood : uint8
{
	Content UMETA(DisplayName = "Content"),
	Angry UMETA(DisplayName = "Angry"),
	Fear UMETA(DisplayName = "Fear"),
	Enum_Length UMETA(Hidden)
};

UENUM(BlueprintType)
enum class EBiome : uint8
{
	Gras UMETA(DisplayName = "Gras"),
	Beach UMETA(DisplayName = "Beach"),
	Mountain UMETA(DisplayName = "Mountain"),
	Volcano UMETA(DisplayName = "Volcano")
};

UENUM(BlueprintType)
enum class EEcoValue : uint8
{
	Tree UMETA(DisplayName = "Tree"),
	Wildlife UMETA(DisplayName = "Wildlife"),
	Forage UMETA(DisplayName = "Forage")
};

UENUM(BlueprintType)
enum class EGameEnding : uint8
{
	Victory UMETA(DisplayName = "Victory"),
	Defeat UMETA(DisplayName = "Defeat")
};

UENUM(BlueprintType)
enum class EGameStatus : uint8
{
	Lobby UMETA(DisplayName = "Lobby"),
	Loading UMETA(DisplayName = "Loading"),
	Running UMETA(DisplayName = "Running"),
	Ended UMETA(DisplayName = "Victory")
};

UENUM()
enum class ECivilianStatus : uint8
{
	Idle UMETA(DisplayName = "Idle"),
	Moving UMETA(DisplayName = "Moving"),
	Working UMETA(DisplayName = "Working")
};

UENUM()
enum class EArmyStatus : uint8
{
	Idling UMETA(DisplayName = "Idle"),
	MovingToNextTile UMETA(DisplayName = "Moving"),
	RecruitingFromTile UMETA(DisplayName = "Recruiting"),
	Fighting UMETA(DisplayName = "Fighting")
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
	Civilian UMETA(DisplayName = "Civilian"),
	Army UMETA(DisplayName = "Military")
};
