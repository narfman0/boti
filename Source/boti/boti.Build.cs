using UnrealBuildTool;

public class boti : ModuleRules
{
	public boti(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] {
			"Core",
			"CoreUObject",
			"Engine",
			"InputCore",
			"EnhancedInput",
			"AIModule",
			"NavigationSystem"
		});

		PrivateDependencyModuleNames.AddRange(new string[] { });
	}
}
