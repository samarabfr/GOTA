#include "GameplayTagRule.h"

bool FGameplayTagRule::IsValid(const FGameplayTagContainer& GameplayTagContainer) const
{
	switch (TagMode)
	{
	case ERuleMode::MatchAny:
		return GameplayTagContainer.HasAny(ConditionTags);
	case ERuleMode::MatchAll:
		return GameplayTagContainer.HasAll(ConditionTags);
	case ERuleMode::ForbidAny:
		return !GameplayTagContainer.HasAny(ConditionTags);
	case ERuleMode::ForbidExactly:
		return !GameplayTagContainer.HasAll(ConditionTags);
	default:
		return false;
	}
}
