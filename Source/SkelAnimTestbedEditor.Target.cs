using UnrealBuildTool;
using System.Collections.Generic;

public class SkelAnimTestbedEditorTarget : TargetRules
{
	public SkelAnimTestbedEditorTarget( TargetInfo Target) : base(Target)
	{
		Type = TargetType.Editor;
		DefaultBuildSettings = BuildSettingsVersion.V2;
		ExtraModuleNames.AddRange( new string[] { "SkelAnimTestbed" } );
	}
}
