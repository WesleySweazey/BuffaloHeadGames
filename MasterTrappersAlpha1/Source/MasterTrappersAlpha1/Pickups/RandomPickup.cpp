// Fill out your copyright notice in the Description page of Project Settings.

#include "RandomPickup.h"

#include "MasterTrappersAlpha1Character.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"
#include "Runtime/Core/Public/Math/UnrealMathUtility.h"



void ARandomPickup::OnInteract(int pickupIdx)
{
    FString pickup = FString::Printf(TEXT("Picked up: %s"), *Name);
    //GEngine->AddOnScreenDebugMessage(1, 5, FColor::Red, pickup);
    //Destroy();

    //TODO
    // Hide item and add it to the inventory
    // get player first
    AMasterTrappersAlpha1Character* player = Cast<AMasterTrappersAlpha1Character>(GetOwner());

   

    if (player)
    {
        PlayEffects();
        Show(false);
       

    }


    if (pickupIdx == 2)
    {
        player->Server_AddFlashBangNum();
    }
    else if (pickupIdx == 3)
    {
        player->Server_AddGrenadeNum();
    }
    else if (pickupIdx == 4)
    {
        player->Server_AddMolotovNum();
    }
    else if (pickupIdx == 5)
    {
        player->Server_AddNinjaStarNum();
    }
    else if (pickupIdx == 6)
    {
        player->Server_AddThrowingAxeNum();
    }
    else if (pickupIdx == 7)
    {
        player->Server_AddBananaPeelNum();
    }
    else if (pickupIdx == 8)
    {
        player->Server_AddBearTrapNum();
    }
    else if (pickupIdx == 9)
    {
        player->Server_AddBoostTrapNum();
    }
    else if (pickupIdx == 10)
    {
        player->Server_AddC4TrapNum();
    }
    else if (pickupIdx == 11)
    {
        player->Server_AddTripWireTrapNum();
    }
    else if (pickupIdx == 12)
    {
        player->Server_AddWhoopieCushionTrapNum();
    }

}

//void ARandomPickup::Server_OnInteract_Implementation()
//{
//    if (Role == ROLE_Authority)
//    {
//        OnInteract();
//    }
//}
//
//bool AGrenadePickup::Server_OnInteract_Validate()
//{
//    return true;
//}

void ARandomPickup::NotifyActorBeginOverlap(AActor * OtherActor)
{
    Super::NotifyActorBeginOverlap(OtherActor);



    AMasterTrappersAlpha1Character* character = Cast<AMasterTrappersAlpha1Character>(OtherActor);
    this->SetOwner(character);
   /* if (OtherActor == character)
    {
        OnInteract();
    }*/

    //1 - drone
    //2 - flash bang
    //3 - grenade
    //4 - molotov
    //5 - ninja star
    //6 - throwing axe

    //7 - bananapeel
    //8 - bear trap
    //9 - boost trap
    //10 - c4 trap
    //11 - trip wire
    //12 - whoopie cushion
    int random = FMath::RandRange(2, 12);
    //random = 5;
    switch (random)
    {
    case(1):
        OnInteract(1);
        break;
    case(2):
        OnInteract(2);
        break;
    case(3):
        OnInteract(3);
        break;
    case(4):
        OnInteract(4);
        break;
    case(5):
        OnInteract(5);
        break;
    case(6):
        OnInteract(6);
        break;
    case(7):
        OnInteract(7);
        break;
    case(8):
        OnInteract(8);
        break;
    case(9):
        OnInteract(9);
        break;
    case(10):
        OnInteract(10);
        break;
    case(11):
        OnInteract(11);
        break;
    case(12):
        OnInteract(12);
        break;
    default:
        break;
    }

    
}


