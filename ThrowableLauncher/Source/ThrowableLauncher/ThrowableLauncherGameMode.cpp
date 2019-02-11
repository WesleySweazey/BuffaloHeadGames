// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#include "ThrowableLauncherGameMode.h"
#include "ThrowableLauncherHUD.h"
#include "ThrowableLauncherCharacter.h"
#include "UObject/ConstructorHelpers.h"

AThrowableLauncherGameMode::AThrowableLauncherGameMode()
	: Super()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/FirstPersonCPP/Blueprints/FirstPersonCharacter"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;

	// use our custom HUD class
	HUDClass = AThrowableLauncherHUD::StaticClass();
}
