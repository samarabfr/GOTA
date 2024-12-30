// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class GOTA : ModuleRules
{
	public GOTA(ReadOnlyTargetRules Target) : base(Target)
	{
		PrivateDependencyModuleNames.AddRange(new string[] { "LearningAgents" });
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[]
		{
			"Core",
			"CoreUObject",
			"Engine",
			"InputCore",
			"EnhancedInput",
			"GameplayTags",
			"FastNoiseGenerator",
			"FastNoise",
			"Niagara",
			"UMG",
			"Slate",
			"SlateCore",
			"NetCore",
			"StateTreeModule",
			"StateTreeEditorModule",
			"GameplayStateTreeModule",
			"AIModule",
			"NetCore",
			"RHI"
		});

		// Uncomment if you are using Slate UI
		// PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });

		// Uncomment if you are using online features
		// PrivateDependencyModuleNames.Add("OnlineSubsystem");

		// To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
	}
}