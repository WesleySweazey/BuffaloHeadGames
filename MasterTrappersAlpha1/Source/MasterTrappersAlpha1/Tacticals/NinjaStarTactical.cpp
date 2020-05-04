// Fill out your copyright notice in the Description page of Project Settings.

#include "NinjaStarTactical.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/SphereComponent.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "Runtime/Engine/Classes/Particles/ParticleSystemComponent.h"
#include "Runtime//Engine/Classes/Sound/SoundCue.h"
#include "Runtime/Engine/Public/WorldCollision.h"
#include "DestructibleComponent.h"
#include "DestructibleActor.h"
#include "MasterTrappersAlpha1Character.h"
#include "Components/StaticMeshComponent.h"
#include "Engine/World.h"

ANinjaStarTactical::ANinjaStarTactical() :ABaseTactical()
{

    // Use a ProjectileMovementComponent to govern this projectile's movement
    ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileComp"));
    ProjectileMovement->SetUpdatedComponent(GetStaticMeshComponent());
    ProjectileMovement->InitialSpeed = 1000.0f;
    ProjectileMovement->MaxSpeed = 3000.f;
    ProjectileMovement->bRotationFollowsVelocity = false;
    ProjectileMovement->bShouldBounce = true;

    GetStaticMeshComponent()->OnComponentHit.AddDynamic(this, &ANinjaStarTactical::OnHit);
    // Die after 3 seconds by default
    InitialLifeSpan = 3.0f;

    SetReplicates(true);
    SetReplicateMovement(true);
}

void ANinjaStarTactical::BeginPlay()
{
    Super::BeginPlay();

    //FTimerHandle handle;
    //GetWorld()->GetTimerManager().SetTimer(handle, this, &ANinjaStarTactical::Destroy, 5.f, false);  // don't want to loop the explotion, just do once every 5.0 second
}

void ANinjaStarTactical::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
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
                    pawn->Multicast_Die();
                }
                this->Destroy();
            }
        }
    }
}

void ANinjaStarTactical::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    //Radius += 100.0f;
    PitchVal++;
    // smooth rotating every frame
    FRotator NewRotation = FRotator(PitchVal, YawVal, RollVal);

    FQuat QuatRotation = FQuat(NewRotation);

    AddActorLocalRotation(QuatRotation, false, 0, ETeleportType::None);
}

