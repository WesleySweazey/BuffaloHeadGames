// Fill out your copyright notice in the Description page of Project Settings.

#include "LobbyGameMode.h"

#include "TimerManager.h"

#include "OnlineGameInstance.h"

//TODO Week 9: Count how many players have logged in, then Start the Game after 10 Seconds
void ALobbyGameMode::PostLogin(APlayerController* NewPlayer)
{
    //CALL Super Function
	Super::PostLogin(NewPlayer);
    //Pre-Increment NumberOfPlayers
	++NumberOfPlayers;

    //IF NumberOfPlayers GREATER THAN OR EQUAL TO MaxNumberOfPlayers
	if (NumberOfPlayers >= MaxNumberOfPlayers)
	{
        //START the GameStartTimer, to CALL ALobbyGameMode::StartGame after 10 seconds
		GetWorldTimerManager().SetTimer(GameStartTimer, this, &ALobbyGameMode::StartGame, 5);
	}
    //ENDIF
}

//TODO Week 9: Handle when a player Logs out
void ALobbyGameMode::Logout(AController* Exiting)
{
    //CALL Super Logout
	Super::Logout(Exiting);
    //PRE-Decrement NumberOfPlayers
	--NumberOfPlayers;
}

//TODO Week 9: Start the Game
void ALobbyGameMode::StartGame()
{
    //CREATE a auto variable called GameInstance and ASSIGN it to the return value of Cast<UOnlineGameInstance>(GetGameInstance())
	auto GameInstance = Cast<UOnlineGameInstance>(GetGameInstance());

	if (GameInstance == nullptr) return;
    
    //CALL StartSession() on the GameInstance
	GameInstance->StartSession();

	UWorld* World = GetWorld();
	if (!ensure(World != nullptr)) return;
    /** Whether the game perform map travels using SeamlessTravel() which loads in the background and doesn't disconnect clients */
	bUseSeamlessTravel = true;
	
    /*We are hosting the game and load the map in listen mode*/
    //CALL ServerTravel() on the World and pass in "/Game/Levels/Sandbox/TestMap?listen"
    //World->ServerTravel("/Game/Levels/Sandbox/TestMap?listen");
    World->ServerTravel("/Game/FirstPersonCPP/Maps/Alpha2?listen");
    //World->ServerTravel("/Game/FirstPersonCPP/Maps/MeatHouse?listen");
    //World->ServerTravel("/Game/Levels/Sandbox/LV_Sandbox?listen");
    //World->SeamlessTravel("/Game/Levels/Sandbox/TestMap?listen", true);
}
