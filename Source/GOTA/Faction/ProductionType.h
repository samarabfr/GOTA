#pragma once

UENUM(BlueprintType)
enum class EProductionType : uint8
{
	Foraging UMETA(DisplayName = "Foraging"),
	Woodcutting UMETA(DisplayName = "Woodcutting"),
	Hunting UMETA(DisplayName = "Hunting"),
	Converting UMETA(DisplayName = "Converting"),
	Expansion UMETA(DisplayName = "Expansion"),
	MAX UMETA(Hidden) // Sentinel value for enum size
};