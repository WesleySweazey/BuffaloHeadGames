// Fill out your copyright notice in the Description page of Project Settings.

#include "MasterTrappersGameStateBase.h"
#include "Net/UnrealNetwork.h"

AMasterTrappersGameStateBase::AMasterTrappersGameStateBase()
{
    bIsPlayerOneLoggedIn = false;
    bIsPlayerTwoLoggedIn = false;
    numberOfPlayersLoggedIn = 0;
    TeamDeathMatch = false;
    //SetReplicates(true);
}

void AMasterTrappersGameStateBase::Multicast_SetScoreTeamOne_Implementation(int Score)
{
    TeamOneScore = Score;
}

void AMasterTrappersGameStateBase::Multicast_SetScoreTeamTwo_Implementation(int Score)
{
    TeamTwoScore = Score;
}

//TODO Week 7: REPLICATE Variables
void AMasterTrappersGameStateBase::GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    //REPLICATE 
    //DOREPLIFETIME(AFinal_GameStateBase, TeamsEnabled);
    DOREPLIFETIME(AMasterTrappersGameStateBase, TeamOneScore);
    DOREPLIFETIME(AMasterTrappersGameStateBase, TeamTwoScore);

    DOREPLIFETIME(AMasterTrappersGameStateBase, bIsPlayerOneLoggedIn);
    DOREPLIFETIME(AMasterTrappersGameStateBase, bIsPlayerTwoLoggedIn);
    DOREPLIFETIME(AMasterTrappersGameStateBase, numberOfPlayersLoggedIn);

    //DOREPLIFETIME(AMasterTrappersGameStateBase, TeamOneSize);
    //DOREPLIFETIME(AMasterTrappersGameStateBase, TeamTwoSize);
    //DOREPLIFETIME(ABaseGameState, GameTime);

    DOREPLIFETIME(AMasterTrappersGameStateBase, TeamOneMaterials);
    DOREPLIFETIME(AMasterTrappersGameStateBase, TeamTwoMaterials);
    DOREPLIFETIME(AMasterTrappersGameStateBase, TeamOneMaterials);
}



