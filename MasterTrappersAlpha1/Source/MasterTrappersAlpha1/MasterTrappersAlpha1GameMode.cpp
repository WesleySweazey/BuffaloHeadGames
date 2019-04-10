// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#include "MasterTrappersAlpha1GameMode.h"
#include "MasterTrappersAlpha1HUD.h"
#include "MasterTrappersAlpha1Character.h"
#include "UObject/ConstructorHelpers.h"
#include "Net/UnrealNetwork.h"

AMasterTrappersAlpha1GameMode::AMasterTrappersAlpha1GameMode()
	: Super()
{
	// set default pawn class to our Blueprinted character
	//static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/FirstPersonCPP/Blueprints/BP_MasterTrappersAlpha1Character"));
	//DefaultPawnClass = PlayerPawnClassFinder.Class;

	// use our custom HUD class
	//HUDClass = AMasterTrappersAlpha1HUD::StaticClass();
}

void AMasterTrappersAlpha1GameMode::HandleStartingNewPlayer_Implementation(APlayerController* NewPlayer)
{
    //CALL Super Function
    Super::HandleStartingNewPlayer_Implementation(NewPlayer);
    //CALL HandleNewPlayer() pass in NewPlayer
    HandleNewPlayer(NewPlayer);
}

//TODO Week 7: Handle the new player that has logged in, assign team and net index
void AMasterTrappersAlpha1GameMode::HandleNewPlayer(APlayerController* NewPlayer)
{
    //DECLARE a ACharacterBase* called character and assign it to the Cast of NewPlayer->GetPawn()
    AMasterTrappersAlpha1Character* character = Cast<AMasterTrappersAlpha1Character>(NewPlayer->GetPawn());
    //IF the character is not nullptr
    if (character)
    {
        //Draw a debug message saying character has logged in
        //GEngine->AddOnScreenDebugMessage(-1, 10, FColor::Blue, "Character Logged In");
        //CALL AssignTeams() on the character
        //AFinal_GameStateBase* GameState = Cast<AFinal_GameStateBase>(GetWorld()->GetGameState());
        character->AssignTeams();
        character->Multicast_AssignColors();
        //CALL AssignNetIndex() on the character
        //character->AssignNetIndex();
    }
    //ENDIF
}