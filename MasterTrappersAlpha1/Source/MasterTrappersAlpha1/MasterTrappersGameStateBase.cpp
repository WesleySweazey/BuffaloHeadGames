// Fill out your copyright notice in the Description page of Project Settings.

#include "MasterTrappersGameStateBase.h"
#include "Net/UnrealNetwork.h"
#include "MasterTrappersAlpha1Character.h"
#include "MasterTrappersAlpha1HUD.h"
#include "Kismet/GameplayStatics.h"
#include "Engine.h"

AMasterTrappersGameStateBase::AMasterTrappersGameStateBase()
{
    bIsPlayerOneLoggedIn = false;
    bIsPlayerTwoLoggedIn = false;
    numberOfPlayersLoggedIn = 0;
    TeamDeathMatch = false;
    gameTimeSecs = 0.0f;
    gameTimeMins = 0;
    //SetReplicates(true);
}

//Multicast set score
void AMasterTrappersGameStateBase::Multicast_SetScoreTeamOne_Implementation(int Score)
{
    TeamOneScore = Score;
}

void AMasterTrappersGameStateBase::Multicast_SetScoreTeamTwo_Implementation(int Score)
{
    TeamTwoScore = Score;
}
float AMasterTrappersGameStateBase::AddTime(float val)
{ 
    gameTimeSecs += val;
    if (gameTimeSecs >= 60)
    {
        gameTimeMins++;
        gameTimeSecs = 0;
        if (gameTimeMins >= 6)
        {
            GameDone();
        }
    }
    return gameTimeSecs;
}

//bool AMasterTrappersGameStateBase::Server_GameDone_Validate()
//{
//    return true;
//}

void AMasterTrappersGameStateBase::GameDone()//Implementation()
{
    TArray<AActor*> FirstFoundActors;
    bool tie = false;
    AMasterTrappersAlpha1Character* highestCharacter = nullptr;
    int highScore = 0;
    UGameplayStatics::GetAllActorsOfClass(GetWorld(), AMasterTrappersAlpha1Character::StaticClass(), FirstFoundActors);
    {
        for (int i = 0; i < FirstFoundActors.Num(); i++)
        {
            AMasterTrappersAlpha1Character* pawn = Cast<AMasterTrappersAlpha1Character>(FirstFoundActors[i]);
            if (pawn->GetLead() == true)
            {
                AHUD* temphud = pawn->GetWorld()->GetFirstPlayerController()->GetHUD();
                AMasterTrappersAlpha1HUD* MasterTrappersHUD = (AMasterTrappersAlpha1HUD*)(temphud);
                MasterTrappersHUD->Win();
            }
            else
            {
                AHUD* temphud = pawn->GetWorld()->GetFirstPlayerController()->GetHUD();
                AMasterTrappersAlpha1HUD* MasterTrappersHUD = (AMasterTrappersAlpha1HUD*)(temphud);
                MasterTrappersHUD->Lose();
            }
        }
    }
}

//REPLICATE Variables
void AMasterTrappersGameStateBase::GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    //REPLICATE 
    //DOREPLIFETIME(AFinal_GameStateBase, TeamsEnabled);
    DOREPLIFETIME(AMasterTrappersGameStateBase, gameTimeSecs);
    DOREPLIFETIME(AMasterTrappersGameStateBase, gameTimeMins);
    DOREPLIFETIME(AMasterTrappersGameStateBase, TeamOneScore);
    DOREPLIFETIME(AMasterTrappersGameStateBase, TeamTwoScore);
    DOREPLIFETIME(AMasterTrappersGameStateBase, TeamThreeScore);
    DOREPLIFETIME(AMasterTrappersGameStateBase, TeamFourScore);
    DOREPLIFETIME(AMasterTrappersGameStateBase, TeamFiveScore);

    DOREPLIFETIME(AMasterTrappersGameStateBase, bIsPlayerOneLoggedIn);
    DOREPLIFETIME(AMasterTrappersGameStateBase, bIsPlayerTwoLoggedIn);
    DOREPLIFETIME(AMasterTrappersGameStateBase, numberOfPlayersLoggedIn);

    //DOREPLIFETIME(AMasterTrappersGameStateBase, TeamOneSize);
    //DOREPLIFETIME(AMasterTrappersGameStateBase, TeamTwoSize);
    //DOREPLIFETIME(ABaseGameState, GameTime);

    DOREPLIFETIME(AMasterTrappersGameStateBase, TeamOneMaterials);
    DOREPLIFETIME(AMasterTrappersGameStateBase, TeamTwoMaterials);
    DOREPLIFETIME(AMasterTrappersGameStateBase, TeamThreeMaterials);
    DOREPLIFETIME(AMasterTrappersGameStateBase, TeamFourMaterials);
    DOREPLIFETIME(AMasterTrappersGameStateBase, TeamFiveMaterials);
}



