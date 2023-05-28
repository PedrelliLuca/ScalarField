//reference on UE4 BlankModule https://docs.unrealengine.com/en-US/API/Developer/BlankModule/IBlankModule/index.html

namespace UnrealBuildTool.Rules
{
	public class AIBuildingBlocks : ModuleRules
	{
		public AIBuildingBlocks(ReadOnlyTargetRules Target) : base(Target)
		{
            PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
            
			PublicDependencyModuleNames.AddRange(
				new string[]
				{
                    "Core",
					"CoreUObject",
					"Engine",
					"AIModule",
					"FactionSystem",
					"GameplayTasks",
					"MovementCommand",
					"GameplayAttributes"
                }
				);

			PrivateDependencyModuleNames.AddRange(
				new string[]
				{
                    "SkillSystem",
                    "SkillUserFSM",
					"DamageHandlers",
					"Thermodynamics", 
					"FSM",
				}
				);

			DynamicallyLoadedModuleNames.AddRange(
				new string[]
				{
					// ... add any modules that your module loads dynamically here ...
				}
				);
		}
	}
}
