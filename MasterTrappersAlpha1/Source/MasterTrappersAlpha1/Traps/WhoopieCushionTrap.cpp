// Fill out your copyright notice in the Description page of Project Settings.

#include "WhoopieCushionTrap.h"
#include "Components/SphereComponent.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "Runtime/Engine/Classes/Particles/ParticleSystemComponent.h"
#include "Runtime//Engine/Classes/Sound/SoundCue.h"
#include "Runtime/Engine/Public/WorldCollision.h"
#include "MasterTrappersAlpha1Character.h"
#include "DestructibleActor.h"
#include "DestructibleComponent.h"

AWhoopieCushionTrap::AWhoopieCushionTrap() : ABaseTrap()
{
    SphereComponent = CreateDefaultSubobject<USphereComponent>("Sphere Collision");
    SphereComponent->SetCollisionProfileName("OverlapAllDynamic");
    SphereComponent->OnComponentBeginOverlap.AddDynamic(this,
        &AWhoopieCushionTrap::OnOverlapBegin);
    SphereComponent->AttachTo(RootComponent);
}

// Called when the game starts or when spawned
void AWhoopieCushionTrap::BeginPlay()
{
    Super::BeginPlay();

    DetonationLength = 0.0f;
}

void AWhoopieCushionTrap::Detonate()
{
    bDetonated = true;
    UParticleSystemComponent* Explosion = UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ExplosionParticles, GetActorTransform());
    Explosion->SetRelativeScale3D(FVector(4.f));

    UGameplayStatics::PlaySoundAtLocation(GetWorld(), ExplosionSound, GetActorLocation());

    TArray<FHitResult> HitActors;

    FVector StartTrace = GetActorLocation();
    FVector EndTrace = StartTrace;
    EndTrace.Z += 360.0f;

    FCollisionShape CollisionShape;
    CollisionShape.ShapeType = ECollisionShape::Sphere;
    CollisionShape.SetSphere(Radius);

    if (GetWorld()->SweepMultiByChannel(HitActors, StartTrace, EndTrace, FQuat::FQuat(), ECC_WorldStatic, CollisionShape))
    {
        for (auto Actors = HitActors.CreateIterator(); Actors; Actors++)
        {
            /*UStaticMeshComponent* SM = Cast<UStaticMeshComponent>((*Actors).Actor->GetRootComponent());
            ADestructibleActor* DA = Cast<ADestructibleActor>((*Actors).GetActor());

            if (SM)
            {
                SM->AddRadialImpulse(GetActorLocation(), 1000.0f, 5000.0f, ERadialImpulseFalloff::RIF_Linear, true);
            }
            else if (DA)
            {
                DA->GetDestructibleComponent()->ApplyRadiusDamage(10.0f, Actors->ImpactPoint, 500.0f, 3000.0f, false);
            }*/
        }
    }
    DetonationLength = 0.3f;
}

void AWhoopieCushionTrap::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
    if (bDetonated == true)
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
                    GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red,
                        "AWhoopieCushionTrap Killed - "
                        + collidedCharacters[i]->GetName());
                    //collidedCharacters[i]->Destroy();
                    collidedCharacters[i]->Die();
                }
            }
        }
    }
}

void AWhoopieCushionTrap::OnOverlapBegin(UPrimitiveComponent * OverlappedComponent, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
    if (OtherActor)
    {
        if (OtherActor->ActorHasTag("Player") || OtherActor->ActorHasTag("AI"))
        {
            AMasterTrappersAlpha1Character* pawn = Cast<AMasterTrappersAlpha1Character>(OtherActor);
            if (pawn)
            {
                GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Blue,
                    "AWhoopieCushionTrap::OnOverlapBegin Overlapped with - "
                    + OtherActor->GetName());
                collidedCharacters.Add(pawn);
            }
        }
    }
}



