// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;
using System.Collections.Generic;

public class UEFighterTarget : TargetRules
{
	public UEFighterTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Game;
		DefaultBuildSettings = BuildSettingsVersion.V2;
		bUseUnityBuild = false;
		bUsePCHFiles = false;
		ExtraModuleNames.Add("UEFighter");
	}
}
