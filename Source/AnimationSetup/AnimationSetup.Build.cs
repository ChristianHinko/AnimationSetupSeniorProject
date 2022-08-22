// Fill out your copyright notice in the Description page of Project Settings.

using UnrealBuildTool;

public class AnimationSetup : ModuleRules
{
	public AnimationSetup(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
		PrivatePCHHeaderFile = "Private/AnimationSetupPrivatePCH.h";

		PublicDependencyModuleNames.AddRange(new string[] { "Core" });
		PrivateDependencyModuleNames.AddRange(
			new string[]
			{
				"CoreUObject",
				"Engine"
			}
		);
	}
}
