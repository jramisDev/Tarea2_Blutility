// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class Tarea2_Bluetility : ModuleRules
{
	public Tarea2_Bluetility(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
	
		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "EnhancedInput" });

		PrivateDependencyModuleNames.AddRange(new string[] {  });

		if (Target.Type == TargetType.Editor)
		{
			//PrivateDependencyModuleNames.Add("EditorScriptingUtilities");
			//PrivateDependencyModuleNames.Add("EditorSubsystem");
			PrivateDependencyModuleNames.Add("UnrealEd");
			PrivateDependencyModuleNames.Add("AssetTools");
			PrivateDependencyModuleNames.Add("Json");
			PrivateDependencyModuleNames.Add("JsonSerialization");
		}
	}
}
