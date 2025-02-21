// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class Shadow_of_the_Desert : ModuleRules
{
	public Shadow_of_the_Desert(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "EnhancedInput", "AIModule" });
	}
}
