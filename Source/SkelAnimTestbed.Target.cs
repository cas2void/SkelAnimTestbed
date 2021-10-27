using UnrealBuildTool;
using System.Collections.Generic;

public class SkelAnimTestbedTarget : TargetRules
{
	public SkelAnimTestbedTarget( TargetInfo Target) : base(Target)
	{
		Type = TargetType.Game;
		DefaultBuildSettings = BuildSettingsVersion.V2;
		ExtraModuleNames.AddRange( new string[] { "SkelAnimTestbed" } );
	}
}
