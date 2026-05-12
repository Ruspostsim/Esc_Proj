// Fill out your copyright notice in the Description page of Project Settings.

using UnrealBuildTool;
using System.Collections.Generic;

public class WILL_DESIRE_AnimEditorTarget : TargetRules
{
	public WILL_DESIRE_AnimEditorTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Editor;
		DefaultBuildSettings = BuildSettingsVersion.V6;

		ExtraModuleNames.AddRange( new string[] { "WILL_DESIRE_Anim" } );
	}
}
