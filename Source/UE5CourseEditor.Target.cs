// Fill out your copyright notice in the Description page of Project Settings.

using UnrealBuildTool;
using System.Collections.Generic;

public class UE5CourseEditorTarget : TargetRules
{
	public UE5CourseEditorTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Editor;
		DefaultBuildSettings = BuildSettingsVersion.V5;

		ExtraModuleNames.AddRange( new string[] { "UE5Course" } );
		IncludeOrderVersion = EngineIncludeOrderVersion.Latest;
	}
}
