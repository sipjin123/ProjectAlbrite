// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class ProjAlbrite : ModuleRules
{
	public ProjAlbrite(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
		PublicIncludePaths.AddRange(
			new string[] {
				"ProjAlbrite/Public"
			}
		);
		
		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "EnhancedInput", 
			"AIModule", 
			"UMG",
			"NavigationSystem",});
		
		PrivateDependencyModuleNames.AddRange(new string[] 
		{ 
			"GameplayAbilities", 
			"GameplayTags", 
			"GameplayTasks" 
		});
	}
}
