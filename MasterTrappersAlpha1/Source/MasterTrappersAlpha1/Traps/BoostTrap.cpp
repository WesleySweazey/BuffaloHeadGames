// Fill out your copyright notice in the Description page of Project Settings.

#include "BoostTrap.h"
#include "MasterTrappersAlpha1Character.h"
#include "Components/BoxComponent.h"

ABoostTrap::ABoostTrap() : ABaseTrap()
{
    BoxComponent = CreateDefaultSubobject<UBoxComponent>("Box Collision");
    BoxComponent->SetCollisionProfileName("OverlapAllDynamic");
    BoxComponent->OnComponentBeginOverlap.AddDynamic(this,
        &ABoostTrap::OnOverlapBegin);
    BoxComponent->AttachTo(RootComponent);
}

void ABoostTrap::OnOverlapBegin(UPrimitiveComponent * OverlappedComponent, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
    if (OtherActor)
    {
        if (OtherActor->ActorHasTag("Player") || OtherActor->ActorHasTag("AI"))
        {
            AMasterTrappersAlpha1Character* pawn = Cast<AMasterTrappersAlpha1Character>(OtherActor);
            if (pawn)
            {
                GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Blue,
                    "ABoostPadTrap::OnOverlapBegin Overlapped with - "
                    + OtherActor->GetName());
                FVector b = -(this->GetActorRightVector());
                pawn->LaunchCharacter(b*boostSpeed, false, false);
            }
        }
    }
}



