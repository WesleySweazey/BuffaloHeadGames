// Fill out your copyright notice in the Description page of Project Settings.

#include "FireAreaEffect.h"
#include "BaseAreaEffect.h"
#include "Components/SphereComponent.h"
#include "MasterTrappersAlpha1Character.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "Runtime/Engine/Classes/Particles/ParticleSystemComponent.h"
#include "Runtime//Engine/Classes/Sound/SoundCue.h"
#include "Components/HealthComponent.h"
#include "Net/UnrealNetwork.h"
#include "Engine/World.h"
#include "Runtime/Engine/Public/TimerManager.h"
#include "Engine.h"

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
    //Set destory timer
    World->GetTimerManager().SetTimer(LifeTimeHandle, this, &ABaseAreaEffect::Server_Stop, LifeTime, false);
    //Set update timer
    World->GetTimerManager().SetTimer(damageTimeHandle, this, &AFireAreaEffect::Server_CheckCollision, 1, true);
    //->OnComponentHit.AddDynamic(this, &AFireAreaEffect::OnHit);
    PlayEffects();
}
//Plays particle effect
void AFireAreaEffect::PlayEffects()
{
    ParticleZoneComponent = UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), Particles, GetActorTransform(),true);
    ParticleZoneComponent->SetRelativeScale3D(FVector(4.f));
    UGameplayStatics::PlaySoundAtLocation(GetWorld(), ExplosionSound, GetActorLocation());
}

bool AFireAreaEffect::Server_CheckCollision_Validate()
{
    return true;
}
//Check Collision
void AFireAreaEffect::Server_CheckCollision_Implementation()
{
    GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Blue,
        "Checking collision");
    UWorld* const World = GetWorld();
    if (World)
    {
        //Checks collision
        TArray<AActor*> Actors;
        CollisionComp->GetOverlappingActors(Actors, AMasterTrappersAlpha1Character::StaticClass());
        for (int i = 0; i < Actors.Num(); i++)
        {
            AMasterTrappersAlpha1Character* pawn = Cast<AMasterTrappersAlpha1Character>(Actors[i]);
            if (pawn) //player
            {
                pawn->GetComponentHealth()->TakeDamage(50.0f);
                GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Blue,
                    "TakeDamage");
                if (pawn->GetHealth() < 50.0f)
                {
                    TArray<AActor*> FoundActors;
                    UGameplayStatics::GetAllActorsOfClass(World, AMasterTrappersAlpha1Character::StaticClass(), FoundActors);
                    
                    for (int i = 0; i < FoundActors.Num(); i++)
                    {
                        AMasterTrappersAlpha1Character* temp = Cast<AMasterTrappersAlpha1Character>(FoundActors[i]);
                        if (temp->Team == Team)
                        {
                            GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow,
                                "Addscore");
                            //Add to team
                            temp->AddScore();
                            break;
                        }
                    }
                    pawn->Multicast_Die();
                    GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow,
                        "Death");
                }
                
            }
        }
        World->GetTimerManager().SetTimer(damageTimeHandle, this, &AFireAreaEffect::Server_CheckCollision, 1, true);
    }
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

//bool AFireAreaEffect::Server_Stop_Validate()
//{
//    return true;
//}
//
//void AFireAreaEffect::Server_Stop_Implementation()
//{
//    Destroy();
//    ParticleZoneComponent->DestroyComponent();
//}