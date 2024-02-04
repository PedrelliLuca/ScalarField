//reference on UE4 BlankModule https://docs.unrealengine.com/en-US/API/Developer/BlankModule/IBlankModule/index.html

namespace UnrealBuildTool.Rules
{
	public class GameplayUI : ModuleRules
	{
		public GameplayUI(ReadOnlyTargetRules Target) : base(Target)
		{
            PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
            
			PublicDependencyModuleNames.AddRange(
				new string[]
				{
                    "Core",
					"CoreUObject",
					"Engine",
					"UMG", 
					"GameplayAttributes", 
					"AbstractUI",
                }
				);

			PrivateDependencyModuleNames.AddRange(
				new string[]
				{
					"GameplayAttributes", 
					"TacticalPauseSystem", 
					"ThermodynamicsCore", 
					"ThermodynamicsPresenter"
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
