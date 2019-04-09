// Fill out your copyright notice in the Description page of Project Settings.


#include "GrenadePickup.h"

#include "MasterTrappersAlpha1Character.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"




void AGrenadePickup::OnInteract()
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
        if(player->GetCurrentGrenadeNum()<player->GetMaxGrenadeNum())
        player->Server_AddToInventory(this);
        player->Server_AddGrenadeNum();
        
    }

}

//void AGrenadePickup::Server_OnInteract_Implementation()
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

void AGrenadePickup::NotifyActorBeginOverlap(AActor * OtherActor)
{
    Super::NotifyActorBeginOverlap(OtherActor);



    AMasterTrappersAlpha1Character* character = Cast<AMasterTrappersAlpha1Character>(OtherActor);
    this->SetOwner(character);
    if (OtherActor == character)
    {

        //character->bHasKey = true;
        OnInteract();
    }
}