//reference on UE4 BlankModule https://docs.unrealengine.com/en-US/API/Developer/BlankModule/IBlankModule/index.html

namespace UnrealBuildTool.Rules
{
	public class InventoryManipulation : ModuleRules
	{
		public InventoryManipulation(ReadOnlyTargetRules Target) : base(Target)
		{
            PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
            
			PublicDependencyModuleNames.AddRange(
				new string[]
				{
					"Core",
					"CoreUObject",
					"Engine",
					"AbstractUI",
				}
				);

			PrivateDependencyModuleNames.AddRange(
				new string[]
				{
					"TacticalPauseSystem",
					"DeveloperSettings",
					// ... add private dependencies that you statically link with here ...
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
