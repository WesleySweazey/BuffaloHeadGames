// Fill out your copyright notice in the Description page of Project Settings.

#include "BearTrap.h"
#include "MasterTrappersAlpha1Character.h"
#include "Components/SphereComponent.h"
#include "Net/UnrealNetwork.h"
#include "Kismet/GameplayStatics.h"
#include "Engine.h"

ABearTrap::ABearTrap() : ABaseTrap()
{
    SphereComponent = CreateDefaultSubobject<USphereComponent>("Sphere Collision");
    SphereComponent->SetCollisionProfileName("OverlapAllDynamic");
    SphereComponent->OnComponentBeginOverlap.AddDynamic(this,
        &ABearTrap::OnOverlapBegin);
    SphereComponent->AttachTo(RootComponent);

    Tags.Add("Trap");
}

bool ABearTrap::Server_OnHit_Validate()
{
    return true;
}

void ABearTrap::Server_OnHit_Implementation()
{
    //Get all players in scene
    TArray<AActor*> FoundActors;
    UGameplayStatics::GetAllActorsOfClass(GetWorld(), AMasterTrappersAlpha1Character::StaticClass(), FoundActors);

    for (int i = 0; i < FoundActors.Num(); i++)
    {
        AMasterTrappersAlpha1Character* temp = Cast<AMasterTrappersAlpha1Character>(FoundActors[i]);
        //If the trap team equal a players team add score
        if (temp->Team == Team)
        {
            temp->AddScore();
            break;
        }
    }
}

void ABearTrap::OnOverlapBegin(UPrimitiveComponent * OverlappedComponent, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
    if (OtherActor)
    {
        if (OtherActor->ActorHasTag("Player"))
        {
            AMasterTrappersAlpha1Character* pawn = Cast<AMasterTrappersAlpha1Character>(OtherActor);
            if (pawn)
            {
                //Check team
                if (pawn->Team != Team)
                {
                    Server_OnHit();
                    pawn->Multicast_Die();
                }
            }
        }
    }
}



