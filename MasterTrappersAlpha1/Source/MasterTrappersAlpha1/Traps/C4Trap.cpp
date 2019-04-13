// Fill out your copyright notice in the Description page of Project Settings.

#include "C4Trap.h"
#include "Components/SphereComponent.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "Runtime/Engine/Classes/Particles/ParticleSystemComponent.h"
#include "Runtime//Engine/Classes/Sound/SoundCue.h"
#include "Runtime/Engine/Public/WorldCollision.h"
#include "MasterTrappersAlpha1Character.h"
#include "DestructibleActor.h"
#include "DestructibleComponent.h"
#include "Engine/World.h"
#include "Net/UnrealNetwork.h"

AC4Trap::AC4Trap() : ABaseTrap()
{
    SphereComponent = CreateDefaultSubobject<USphereComponent>("Sphere Collision");
    SphereComponent->SetCollisionProfileName("OverlapAllDynamic");
    SphereComponent->OnComponentBeginOverlap.AddDynamic(this,
        &AC4Trap::OnOverlapBegin);
    SphereComponent->AttachTo(RootComponent);
}

// Called when the game starts or when spawned
void AC4Trap::BeginPlay()
{
    Super::BeginPlay();

    DetonationLength = 0.0f;
}

void AC4Trap::Detonate()
{
    bDetonated = true;
    m_Explosion = UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ExplosionParticles, GetActorTransform());
    m_Explosion->SetRelativeScale3D(FVector(4.f));
    m_Explosion->SetIsReplicated(true);
    UWorld* const World = GetWorld();
    if(World)
    UGameplayStatics::PlaySoundAtLocation(World, ExplosionSound, GetActorLocation());

    TArray<AActor*> Actors;
    SphereComponent->GetOverlappingActors(Actors, AMasterTrappersAlpha1Character::StaticClass());
    for (int i = 0; i < Actors.Num(); i++)
    {
        AMasterTrappersAlpha1Character* pawn = Cast<AMasterTrappersAlpha1Character>(Actors[i]);
        if (pawn) //if the flash bang not hitting the player himself
        {
            TArray<AActor*> FoundActors;
            UGameplayStatics::GetAllActorsOfClass(GetWorld(), AMasterTrappersAlpha1Character::StaticClass(), FoundActors);

            for (int j = 0; j < FoundActors.Num(); j++)
            {
                AMasterTrappersAlpha1Character* temp = Cast<AMasterTrappersAlpha1Character>(FoundActors[j]);
                if (temp->Team == Team)
                {
                    temp->AddScore();
                    break;
                }
            }
            pawn->Multicast_Die();
        }
    }

    //TArray<FHitResult> HitActors;

    //FVector StartTrace = GetActorLocation();
    //FVector EndTrace = StartTrace;
    //EndTrace.Z += 360.0f;

    //FCollisionShape CollisionShape;
    //CollisionShape.ShapeType = ECollisionShape::Sphere;
    //CollisionShape.SetSphere(Radius);

    //if (GetWorld()->SweepMultiByChannel(HitActors, StartTrace, EndTrace, FQuat::FQuat(), ECC_WorldStatic, CollisionShape))
    //{
    //    for (auto Actors = HitActors.CreateIterator(); Actors; Actors++)
    //    {
    //        /*UStaticMeshComponent* SM = Cast<UStaticMeshComponent>((*Actors).Actor->GetRootComponent());
    //        ADestructibleActor* DA = Cast<ADestructibleActor>((*Actors).GetActor());

    //        if (SM)
    //        {
    //            SM->AddRadialImpulse(GetActorLocation(), 1000.0f, 5000.0f, ERadialImpulseFalloff::RIF_Linear, true);
    //        }
    //        else if (DA)
    //        {
    //            DA->GetDestructibleComponent()->ApplyRadiusDamage(10.0f, Actors->ImpactPoint, 500.0f, 3000.0f, false);
    //        }*/
    //    }
    //}
    //DetonationLength = 0.3f;
}

void AC4Trap::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
    /*if (bDetonated == true)
    {
        if (DetonationLength < 0)
        {
            Destroy();
        }
        else
        {
            DetonationLength -= DeltaTime;
            for (int i = 0; i < collidedCharacters.Num(); i++)
            {
                if (IsOverlappingActor(collidedCharacters[i]))
                {
                    TArray<AActor*> FoundActors;
                    UGameplayStatics::GetAllActorsOfClass(GetWorld(), AMasterTrappersAlpha1Character::StaticClass(), FoundActors);

                    for (int i = 0; i < FoundActors.Num(); i++)
                    {
                        AMasterTrappersAlpha1Character* temp = Cast<AMasterTrappersAlpha1Character>(FoundActors[i]);
                        if (temp->Team == Team)
                        {
                            temp->AddScore();
                            break;
                        }
                    }
                    collidedCharacters[i]->Multicast_Die();
                }
            }
        }
    }*/
}

void AC4Trap::OnOverlapBegin(UPrimitiveComponent * OverlappedComponent, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
    if (OtherActor)
    {
        if (OtherActor->ActorHasTag("Player"))
        {
            AMasterTrappersAlpha1Character* pawn = Cast<AMasterTrappersAlpha1Character>(OtherActor);
            if (pawn)
            {
                collidedCharacters.Add(pawn);
            }
        }
    }
}
//Replicates UPROPERTIES
void AC4Trap::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    DOREPLIFETIME(AC4Trap, m_Explosion);
}



