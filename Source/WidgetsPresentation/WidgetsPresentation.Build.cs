//reference on UE4 BlankModule https://docs.unrealengine.com/en-US/API/Developer/BlankModule/IBlankModule/index.html

namespace UnrealBuildTool.Rules
{
	public class WidgetsPresentation : ModuleRules
	{
		public WidgetsPresentation(ReadOnlyTargetRules Target) : base(Target)
		{
            PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
            
			PublicDependencyModuleNames.AddRange(
				new string[]
				{
                    "Core",
					"CoreUObject",
					"Engine",
					"GameplayUI",
					"InventorySystem",
                }
				);

			PrivateDependencyModuleNames.AddRange(
				new string[]
				{
					"UMG"
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
