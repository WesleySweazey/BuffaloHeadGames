// Fill out your copyright notice in the Description page of Project Settings.

#include "GasAreaEffect.h"
#include "BaseAreaEffect.h"
#include "Components/SphereComponent.h"
#include "MasterTrappersAlpha1Character.h"

// Called when the game starts or when spawned
void AGasAreaEffect::BeginPlay()
{
    Super::BeginPlay();
    UWorld* const World = GetWorld();
    World->GetTimerManager().SetTimer(LifeTimeHandle, this, &ABaseAreaEffect::Stop, LifeTime, false);
    CollisionComp->OnComponentHit.AddDynamic(this, &AGasAreaEffect::OnHit);
    ABaseAreaEffect::PlayEffects();
}

// Called every frame
void AGasAreaEffect::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

}

void AGasAreaEffect::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
    // Checks other actor
    if (OtherActor)
    {
        // Checks other actor if player
        if (OtherActor->ActorHasTag("Player"))
        {
            // Cast other actor to player
            AMasterTrappersAlpha1Character* pawn = Cast<AMasterTrappersAlpha1Character>(OtherActor);
            if (pawn)
            {
                GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Green,
                    "AGasAreaEffect::OnOverlapBegin Overlapped with - "
                    + OtherActor->GetName());
                pawn->GetHealth();
                //Get Health Component
            }
        }
    }
}


