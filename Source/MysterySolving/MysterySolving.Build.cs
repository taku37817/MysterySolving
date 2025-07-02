// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class MysterySolving : ModuleRules
{
	public MysterySolving(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "HeadMountedDisplay", "UMG", "PhysicsCore", "Niagara", "Slate", "SlateCore" });
	}
}
