// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#include "MasterTrappersAlpha1GameMode.h"
#include "MasterTrappersAlpha1HUD.h"
#include "MasterTrappersAlpha1Character.h"
#include "UObject/ConstructorHelpers.h"

AMasterTrappersAlpha1GameMode::AMasterTrappersAlpha1GameMode()
	: Super()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/FirstPersonCPP/Blueprints/BP_MasterTrappersAlpha1Character"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;

	// use our custom HUD class
	HUDClass = AMasterTrappersAlpha1HUD::StaticClass();
}
