// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;
using System.Collections.Generic;

public class MasterTrappersAlpha1EditorTarget : TargetRules
{
	public MasterTrappersAlpha1EditorTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Editor;
		ExtraModuleNames.Add("MasterTrappersAlpha1");
	}
}
