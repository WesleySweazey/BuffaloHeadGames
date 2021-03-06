// Fill out your copyright notice in the Description page of Project Settings.

#include "BaseAreaEffect.h"
#include "Components/SphereComponent.h"
#include "MasterTrappersAlpha1Character.h"
#include "Components/StaticMeshComponent.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "Runtime/Engine/Classes/Particles/ParticleSystemComponent.h"
#include "Runtime//Engine/Classes/Sound/SoundCue.h"
#include "Net/UnrealNetwork.h"
#include "Engine/World.h"
#include "Runtime/Engine/Public/TimerManager.h"

// Sets default values
ABaseAreaEffect::ABaseAreaEffect()
{
    // Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = true;
    //StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>("Static Mesh Component");
    USceneComponent* SceneComponent = CreateDefaultSubobject<USceneComponent>("Base Scene Component");
    Particles = CreateDefaultSubobject<UParticleSystem>("Fire Particle Component");
    RootComponent = SceneComponent;
    
    CollisionComp = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComp"));
    CollisionComp->InitSphereRadius(5.0f);
    CollisionComp->SetCollisionProfileName("OverlapAllDynamic");
    //CollisionComp->BodyInstance.SetCollisionProfileName("Projectile");
    //CollisionComp->OnComponentHit.AddDynamic(this, &ABaseAreaEffect::OnHit);
    CollisionComp->SetupAttachment(RootComponent);
    Tags.Add("AreaEffect");
    SetReplicates(true);
}
//Plays particle effect
void ABaseAreaEffect::PlayEffects()
{
    ParticleZoneComponent = UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), Particles, GetActorTransform());
    ParticleZoneComponent->SetRelativeScale3D(FVector(4.f));
    UGameplayStatics::PlaySoundAtLocation(GetWorld(), ExplosionSound, GetActorLocation());
}

// Called when the game starts or when spawned
void ABaseAreaEffect::BeginPlay()
{
    Super::BeginPlay();
    UWorld* const World = GetWorld();
    World->GetTimerManager().SetTimer(LifeTimeHandle, this, &ABaseAreaEffect::Server_Stop, LifeTime, false);
    PlayEffects();
}

//bool ABaseAreaEffect::Server_CheckCollision_Validate()
//{
//    return true;
//}
////Pure void check collision
//void ABaseAreaEffect::Server_CheckCollision_Implementation()
//{
//
//}

// Called every frame
void ABaseAreaEffect::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

}

void ABaseAreaEffect::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
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
                /*GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Blue,
                    "ABaseAreaEffect::OnOverlapBegin Overlapped with - "
                    + OtherActor->GetName());*/
                //pawn->Server_StartStun();
                //this->Destroy();
            }
        }
    }
}
//stop particle effects
bool ABaseAreaEffect::Server_Stop_Validate()
{
    return true;
}

void ABaseAreaEffect::Server_Stop_Implementation()
{
    Destroy();
    if (ParticleZoneComponent)
    {
        ParticleZoneComponent->DestroyComponent();
    }
}
//Replications
void ABaseAreaEffect::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    DOREPLIFETIME(ABaseAreaEffect, Team);
    DOREPLIFETIME(ABaseAreaEffect, Particles);
    DOREPLIFETIME(ABaseAreaEffect, LifeTime);
    DOREPLIFETIME(ABaseAreaEffect, ParticleZoneComponent);
    DOREPLIFETIME(ABaseAreaEffect, ExplosionSound);
}