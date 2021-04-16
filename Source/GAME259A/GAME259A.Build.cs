// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class GAME259A : ModuleRules
{
    public GAME259A(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

        bEnableExceptions = true;

        PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "HeadMountedDisplay", "Steamworks", "OnlineSubsystemUtils", "OnlineSubsystem", "AdvancedSessions", "AdvancedSteamSessions" });
        PrivateDependencyModuleNames.AddRange(new string[] { "OnlineSubsystem", "AdvancedSessions", "AdvancedSteamSessions" });
        PrivateIncludePathModuleNames.AddRange(new string[] { "AdvancedSessions", "AdvancedSteamSessions" });
    }
}
