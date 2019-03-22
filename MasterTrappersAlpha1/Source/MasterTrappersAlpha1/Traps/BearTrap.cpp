// Fill out your copyright notice in the Description page of Project Settings.

#include "BearTrap.h"
#include "MasterTrappersAlpha1Character.h"
#include "Components/SphereComponent.h"

ABearTrap::ABearTrap() : ABaseTrap()
{
    SphereComponent = CreateDefaultSubobject<USphereComponent>("Sphere Collision");
    SphereComponent->SetCollisionProfileName("OverlapAllDynamic");
    SphereComponent->OnComponentBeginOverlap.AddDynamic(this,
        &ABearTrap::OnOverlapBegin);
    SphereComponent->AttachTo(RootComponent);
}

void ABearTrap::OnOverlapBegin(UPrimitiveComponent * OverlappedComponent, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
    if (OtherActor)
    {
        if (OtherActor->ActorHasTag("Player") || OtherActor->ActorHasTag("AI"))
        {
            AMasterTrappersAlpha1Character* pawn = Cast<AMasterTrappersAlpha1Character>(OtherActor);
            if (pawn)
            {
                GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Blue,
                    "ABearTrap::OnOverlapBegin Overlapped with - "
                    + OtherActor->GetName());
                //pawn->Destroy();
                pawn->Die();
            }
        }
    }
}



