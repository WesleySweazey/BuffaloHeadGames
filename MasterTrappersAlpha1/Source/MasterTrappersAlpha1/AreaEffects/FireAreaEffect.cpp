// Fill out your copyright notice in the Description page of Project Settings.

#include "FireAreaEffect.h"
#include "BaseAreaEffect.h"
#include "Components/SphereComponent.h"
#include "MasterTrappersAlpha1Character.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "Runtime/Engine/Classes/Particles/ParticleSystemComponent.h"
#include "Runtime//Engine/Classes/Sound/SoundCue.h"
#include "Components/HealthComponent.h"

AFireAreaEffect::AFireAreaEffect() : ABaseAreaEffect()
{
    //Super::ABaseAreaEffect();
    SetReplicates(true);
}

// Called when the game starts or when spawned
void AFireAreaEffect::BeginPlay()
{
    //Super::BeginPlay();
    UWorld* const World = GetWorld();
    World->GetTimerManager().SetTimer(LifeTimeHandle, this, &AFireAreaEffect::Stop, LifeTime, false);
    //->OnComponentHit.AddDynamic(this, &AFireAreaEffect::OnHit);
    PlayEffects();
}

void AFireAreaEffect::PlayEffects()
{
    ParticleZoneComponent = UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), Particles, GetActorTransform());
    ParticleZoneComponent->SetRelativeScale3D(FVector(4.f));
    UGameplayStatics::PlaySoundAtLocation(GetWorld(), ExplosionSound, GetActorLocation());
}

// Called every frame
void AFireAreaEffect::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

}

void AFireAreaEffect::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
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
                //Get Health Component
                pawn->GetComponentHealth()->TakeDamage(5.0f);
                if (pawn->GetHealth() < 0.0f)
                {
                    //Get character Actors
                    TArray<AActor*> FoundActors;
                    UGameplayStatics::GetAllActorsOfClass(GetWorld(), AMasterTrappersAlpha1Character::StaticClass(), FoundActors);

                    for (int i = 0; i < FoundActors.Num(); i++)
                    {
                        AMasterTrappersAlpha1Character* temp = Cast<AMasterTrappersAlpha1Character>(FoundActors[i]);
                        if (temp->Team == Team)
                        {
                            //Add to team
                            temp->AddScore();
                            break;
                        }
                    }
                    pawn->Multicast_Die();
                }
            }
        }
    }
}

void AFireAreaEffect::Stop()
{
    Destroy();
    ParticleZoneComponent->DestroyComponent();
}