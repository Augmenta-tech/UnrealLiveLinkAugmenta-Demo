// Copyright Augmenta, All Rights Reserved.

using UnrealBuildTool;
using System.Collections.Generic;

public class LiveLinkAugmentaDemoTarget : TargetRules
{
	public LiveLinkAugmentaDemoTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Game;
		DefaultBuildSettings = BuildSettingsVersion.V5;

		ExtraModuleNames.AddRange( new string[] { "LiveLinkAugmentaDemo" } );
	}
}
