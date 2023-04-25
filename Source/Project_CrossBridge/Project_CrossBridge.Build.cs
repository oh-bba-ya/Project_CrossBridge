// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class Project_CrossBridge : ModuleRules
{
	public Project_CrossBridge(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
	
		PublicDependencyModuleNames.AddRange(new string[] {"Core", "CoreUObject", "Engine", "InputCore", "EnhancedInput","UMG","AnimGraphRuntime", "HeadMountedDisplay", "MultiplayerSessions","Niagara","OnlineSubsystem","OnlineSubsystemSteam"});

		PrivateDependencyModuleNames.AddRange(new string[] {  });

		// Uncomment if you are using Slate UI
		// PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });
		
		// Uncomment if you are using online features
		// PrivateDependencyModuleNames.Add("OnlineSubsystem");

		// To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
	}
}
