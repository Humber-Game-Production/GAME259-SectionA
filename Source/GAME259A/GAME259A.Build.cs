// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class GAME259A : ModuleRules
{
	public GAME259A(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "HeadMountedDisplay", "Steamworks", "OnlineSubsystemUtils", "OnlineSubsystem" });

		PrivateDependencyModuleNames.AddRange(new string[] { "AdvancedSessions", "AdvancedSteamSessions" });
		PrivateIncludePathModuleNames.AddRange(new string[] { "AdvancedSessions", "AdvancedSteamSessions" });
	}
}
