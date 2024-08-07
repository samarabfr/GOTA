#pragma once

UENUM(BlueprintType)
enum class EProductionType : uint8
{
	Foraging UMETA(DisplayName = "Foraging"),
	Woodcutting UMETA(DisplayName = "Woodcutting"),
	Hunting UMETA(DisplayName = "Hunting"),
	Converting UMETA(DisplayName = "Converting"),
	Expansion UMETA(DisplayName = "Expansion"),
	Stonecutting UMETA(DisplayName = "Stonecutting"),
	Musketmaking UMETA(DisplayName = "Musketmaking"),
	Shieldmaking UMETA(DisplayName = "Shieldmaking"),
	Bowmaking UMETA(DisplayName = "Bowmaking"),
	MAX UMETA(Hidden) // Sentinel value for enum size
};

UENUM(BlueprintType)
enum class ECultureLoyalty : uint8
{
	Colonists UMETA(DisplayName = "Colonists"),
	Guardian1 UMETA(DisplayName = "Guardian1"),
	Guardian2 UMETA(DisplayName = "Guardian2"),
	Guardian3 UMETA(DisplayName = "Guardian3"),
	Guardian4 UMETA(DisplayName = "Guardian4"),
	MAX UMETA(Hidden) // Sentinel value for enum size
};

UENUM(BlueprintType)
enum class EMood : uint8
{
	Neutral UMETA(DisplayName = "Neutral"),
	Fearful UMETA(DisplayName = "Fearful"),
	Aggressive UMETA(DisplayName = "Aggressive"),
	MAX UMETA(Hidden) // Sentinel value for enum size
};

UENUM(BlueprintType)
enum class EAffiliation : uint8
{
	Ally UMETA(DisplayName = "Ally"),
	Enemy UMETA(DisplayName = "Enemy")
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
