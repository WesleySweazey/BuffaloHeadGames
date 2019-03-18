// Fill out your copyright notice in the Description page of Project Settings.

#include "GrenadePickup.h"
#include "MasterTrappersAlpha1Character.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"


void AGrenadePickup::OnInteract()
{
    FString pickup = FString::Printf(TEXT("Picked up: %s"), *Name);
    GEngine->AddOnScreenDebugMessage(1, 5, FColor::Red, pickup);
    //Destroy();

    //TODO
    // Hide item and add it to the inventory
    // get player first
    AMasterTrappersAlpha1Character* player = Cast<AMasterTrappersAlpha1Character>(UGameplayStatics::GetPlayerCharacter(this, 0));
    if (player)
    {
        PlayEffects();
        Show(false);
        player->AddToInventory(this);
        player->AddGrenadeNum();
    }

}

void AGrenadePickup::NotifyActorBeginOverlap(AActor * OtherActor)
{
    Super::NotifyActorBeginOverlap(OtherActor);



    AMasterTrappersAlpha1Character* character = Cast<AMasterTrappersAlpha1Character>(OtherActor);
    if (OtherActor == character)
    {

        //character->bHasKey = true;
        OnInteract();
    }
}