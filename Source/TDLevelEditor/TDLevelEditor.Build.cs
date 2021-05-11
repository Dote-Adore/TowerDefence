﻿// Copyright Epic Games, Inc. All Rights Reserved.

namespace UnrealBuildTool.Rules
{
	public class TDLevelEditor : ModuleRules
	{
		public TDLevelEditor(ReadOnlyTargetRules Target) : base(Target)
		{
			PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
		
			PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore" });

			PrivateDependencyModuleNames.AddRange(new string[] { "ToolMenus","EditorStyle","PropertyEditor","EditorWidgets", "UnrealEd","TowerDefence","Settings" });
		}
	}
}