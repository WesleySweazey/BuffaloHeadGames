// Fill out your copyright notice in the Description page of Project Settings.

#include "BananaPeelTrap.h"
#include "MasterTrappersAlpha1Character.h"
#include "Components/SphereComponent.h"

ABananaPeelTrap::ABananaPeelTrap() : ABaseTrap()
{
    SphereComponent = CreateDefaultSubobject<USphereComponent>("Sphere Collision");
    SphereComponent->SetCollisionProfileName("OverlapAllDynamic");
    SphereComponent->OnComponentBeginOverlap.AddDynamic(this,
        &ABananaPeelTrap::OnOverlapBegin);
    SphereComponent->AttachTo(RootComponent);
}

void ABananaPeelTrap::OnOverlapBegin(UPrimitiveComponent * OverlappedComponent, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
    if (OtherActor)
    {
        if (OtherActor->ActorHasTag("Player") || OtherActor->ActorHasTag("AI"))
        {
            AMasterTrappersAlpha1Character* pawn = Cast<AMasterTrappersAlpha1Character>(OtherActor);
            if (pawn)
            {
                GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Blue,
                    "ABananaPeelTrap::OnOverlapBegin Overlapped with - "
                    + OtherActor->GetName());
                pawn->StartSlip();
                this->Destroy();
            }
        }
    }
}



