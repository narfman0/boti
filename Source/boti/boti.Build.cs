using UnrealBuildTool;

public class boti : ModuleRules
{
	public boti(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		// Expose all subdirectories under the module root for cross-folder includes.
		PublicIncludePaths.AddRange(new string[] { ModuleDirectory });

		PublicDependencyModuleNames.AddRange(new string[] {
			"Core",
			"CoreUObject",
			"Engine",
			"InputCore",
			"EnhancedInput",
			"AIModule",
			"NavigationSystem",
			"Niagara"
		});

		PrivateDependencyModuleNames.AddRange(new string[] { });
	}
}
