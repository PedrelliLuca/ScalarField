// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class ScalarField : ModuleRules
{
	public ScalarField(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicDependencyModuleNames.AddRange(new string[] { 
			"Core", 
			"CoreUObject", 
			"Engine", 
			"InputCore", 
			"HeadMountedDisplay", 
			"NavigationSystem", 
			"AIModule", 
			"Niagara",
			"EnvironmentGrid",
			"SkillSystem",
			"Thermodynamics",
			"SkillUserFSM",
		});
    }
}
