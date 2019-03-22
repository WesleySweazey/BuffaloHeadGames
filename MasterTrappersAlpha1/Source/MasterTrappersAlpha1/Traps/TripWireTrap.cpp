// Fill out your copyright notice in the Description page of Project Settings.

#include "TripWireTrap.h"
#include "BaseTrap.h"
#include "MasterTrappersAlpha1Character.h"
#include "Components/BoxComponent.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "Runtime/Engine/Classes/Particles/ParticleSystemComponent.h"
#include "Runtime//Engine/Classes/Sound/SoundCue.h"
#include "Runtime/Engine/Public/WorldCollision.h"

ATripWireTrap::ATripWireTrap() : ABaseTrap()
{
    BoxComponent = CreateDefaultSubobject<UBoxComponent>("Box Collision");
    BoxComponent->SetCollisionProfileName("OverlapAllDynamic");
    BoxComponent->OnComponentBeginOverlap.AddDynamic(this,
        &ATripWireTrap::OnOverlapBegin);
    BoxComponent->AttachTo(RootComponent);
}

// Called when the game starts or when spawned
void ATripWireTrap::BeginPlay()
{
    Super::BeginPlay();
    GetStaticMeshComponent()->SetCollisionProfileName("NoCollision");
}

void ATripWireTrap::OnOverlapBegin(UPrimitiveComponent * OverlappedComponent, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
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
                //pawn->IncrementPickupCount(1);
                /*FVector Character = pawn->GetVelocity();
                pawn->LaunchCharacter(Character * 10, false, false);*/
                //pawn->Destroy();

                UParticleSystemComponent* Explosion = UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ExplosionParticles, GetActorTransform());
                Explosion->SetRelativeScale3D(FVector(4.f));

                UGameplayStatics::PlaySoundAtLocation(GetWorld(), ExplosionSound, GetActorLocation());
                Destroy();
                
               pawn->Die();
            }
        }
    }
}



