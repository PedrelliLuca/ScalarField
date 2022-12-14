//reference on UE4 BlankModule https://docs.unrealengine.com/en-US/API/Developer/BlankModule/IBlankModule/index.html

namespace UnrealBuildTool.Rules
{
	public class SkillUserFSM : ModuleRules
	{
		public SkillUserFSM(ReadOnlyTargetRules Target) : base(Target)
		{
            PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
            
			PublicDependencyModuleNames.AddRange(
				new string[]
				{
                    "Core",
                    "CoreUObject",
                    "Engine",
					"SkillSystem",
					"MovementCommand",
					"GameplayAttributes",
				}
				);

			PrivateDependencyModuleNames.AddRange(
				new string[]
				{
					"InteractionSystem", 
					"InventorySystem",
                    "WidgetsPresentation",
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
