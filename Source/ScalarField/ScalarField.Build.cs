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
            "DeveloperSettings",
            "EnvironmentGrid",
            "SkillSystem",
            "Thermodynamics",
            "SkillUserFSM",
            "MovementCommand",
            "TacticalPauseSystem",
            "GameplayAttributes",
            "DamageHandlers",
            "UMG",
            "InteractionSystem",
            "InventoryCore",
            "WidgetsPresentation",
            "Pickups",
            "InventoryManipulation",
            "InventoryPresenter",
            "AIModule",
            "GameplayTasks",
            "SkillSpawnedEntities",
            "FactionSystem",
            "AIControllers",
            "AIBuildingBlocks",
            "SkillCastConditions",
            "NewSkillSystem",
            "FSM",
            "ConcreteSkills",
        });

        PrivateDependencyModuleNames.AddRange(new string[] {
            "SkillTargeting"
        });
    }
}
