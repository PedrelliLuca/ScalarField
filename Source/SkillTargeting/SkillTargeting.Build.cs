//reference on UE4 BlankModule https://docs.unrealengine.com/en-US/API/Developer/BlankModule/IBlankModule/index.html

namespace UnrealBuildTool.Rules
{
	public class SkillTargeting : ModuleRules
	{
		public SkillTargeting(ReadOnlyTargetRules Target) : base(Target)
		{
            PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
            
			PublicDependencyModuleNames.AddRange(
				new string[]
				{
					"Core",
					"CoreUObject",
					"Engine",
					"NewSkillSystem",
				}
				);

			PrivateDependencyModuleNames.AddRange(
				new string[]
				{
					"MaterialSystem",
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
