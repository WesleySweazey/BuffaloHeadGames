// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "MasterTrappersGameStateBase.generated.h"

/**
 * 
 */
UCLASS()
class MASTERTRAPPERSALPHA1_API AMasterTrappersGameStateBase : public AGameStateBase
{
	GENERATED_BODY()
	
public:
    AMasterTrappersGameStateBase();

    bool TeamDeathMatch;

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Gameplay, meta = (ClampMin = 0), Replicated)
        int TeamOneScore;

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Gameplay, meta = (ClampMin = 0), Replicated)
        int TeamTwoScore;

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Gameplay, meta = (ClampMin = 0), Replicated)
        int TeamThreeScore;

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Gameplay, meta = (ClampMin = 0), Replicated)
        int TeamFourScore;

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Gameplay, meta = (ClampMin = 0), Replicated)
        int TeamFiveScore;

    //Getters for Score
    int GetScoreTeamOne() { return TeamOneScore; }
    int GetScoreTeamTwo() { return TeamTwoScore; }
    int GetScoreTeamThree() { return TeamThreeScore; }
    int GetScoreTeamFour() { return TeamFourScore; }
    int GetScoreTeamFive() { return TeamFiveScore; }

    UFUNCTION(NetMulticast, Reliable)
        void Multicast_SetScoreTeamOne(int Score);
    UFUNCTION(NetMulticast, Reliable)
        void Multicast_SetScoreTeamTwo(int Score);

    //UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Materials, Replicated)
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Materials, Replicated)
        class UMaterialInterface* TeamOneMaterials;

    //UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Materials, Replicated)
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Materials, Replicated)
        class UMaterialInterface* TeamTwoMaterials;

    //UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Materials, Replicated)
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Materials, Replicated)
        class UMaterialInterface* TeamThreeMaterials;

    //UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Materials, Replicated)
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Materials, Replicated)
        class UMaterialInterface* TeamFourMaterials;

    //UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Materials, Replicated)
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Materials, Replicated)
        class UMaterialInterface* TeamFiveMaterials;

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Gameplay, Replicated)
        int numberOfPlayersLoggedIn;

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Gameplay, Replicated)
        bool bIsPlayerOneLoggedIn;

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Gameplay, Replicated)
        bool bIsPlayerTwoLoggedIn;
	
    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Gameplay, Replicated)
        bool bIsPlayerThreeLoggedIn;

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Gameplay, Replicated)
        bool bIsPlayerFourLoggedIn;

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Gameplay, Replicated)
        bool bIsPlayerFiveLoggedIn;
};
