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
            "Thermodynamics",
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
            "ConcreteItems",
            "ConcreteInteractables",
            "AbstractUI",
            "ThermodynamicsCore",
            "GameplayUI",
        });

        PrivateDependencyModuleNames.AddRange(new string[] {
            "SkillTargeting"
        });
    }
}
