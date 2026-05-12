// Fill out your copyright notice in the Description page of Project Settings.

using UnrealBuildTool;
using System.Collections.Generic;

public class WILL_DESIRE_AnimTarget : TargetRules
{
	public WILL_DESIRE_AnimTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Game;
		DefaultBuildSettings = BuildSettingsVersion.V6;

		ExtraModuleNames.AddRange( new string[] { "WILL_DESIRE_Anim" } );
	}
}
