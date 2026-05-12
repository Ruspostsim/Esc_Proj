// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class WebcamMedia : ModuleRules
{
	public WebcamMedia(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[]
		{
			"Core",
			"CoreUObject",
			"Engine",
			"MediaAssets",
			"RenderCore",
			"RHI"
		});

		PrivateDependencyModuleNames.AddRange(new string[]
		{
			"MediaUtils"
		});
	}
}
