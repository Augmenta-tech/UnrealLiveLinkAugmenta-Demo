// Copyright Augmenta, All Rights Reserved.

using UnrealBuildTool;
using System.Collections.Generic;

public class LiveLinkAugmentaDemoEditorTarget : TargetRules
{
	public LiveLinkAugmentaDemoEditorTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Editor;
		DefaultBuildSettings = BuildSettingsVersion.V5;
		IncludeOrderVersion = EngineIncludeOrderVersion.Latest;

		ExtraModuleNames.AddRange( new string[] { "LiveLinkAugmentaDemo" } );
	}
}
