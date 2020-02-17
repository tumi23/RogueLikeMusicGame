// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class RogueLikeMusicGame : ModuleRules
{
	public RogueLikeMusicGame(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "HeadMountedDisplay", "SoundVisualizations"});
        PrivateDependencyModuleNames.AddRange(new string[] {"SoundVisualizations"});
    }
}
