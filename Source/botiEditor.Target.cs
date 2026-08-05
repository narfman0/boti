using UnrealBuildTool;
using System.Collections.Generic;

public class botiEditorTarget : TargetRules
{
	public botiEditorTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Editor;
		DefaultBuildSettings = BuildSettingsVersion.V7;
		IncludeOrderVersion = EngineIncludeOrderVersion.Latest;
		ExtraModuleNames.Add("boti");
	}
}
