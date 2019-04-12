// Fill out your copyright notice in the Description page of Project Settings.

#include "SmokeAreaEffect.h"
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

ASmokeAreaEffect::ASmokeAreaEffect() : ABaseAreaEffect()
{
    //Super::ABaseAreaEffect();
    SetReplicates(true);
}

// Called when the game starts or when spawned
void ASmokeAreaEffect::BeginPlay()
{
    LifeTime = 5.0f;
    //Super::BeginPlay();
    UWorld* const World = GetWorld();
    //Set destory timer
    World->GetTimerManager().SetTimer(LifeTimeHandle, this, &ABaseAreaEffect::Server_Stop, LifeTime, false);
    //->OnComponentHit.AddDynamic(this, &AFireAreaEffect::OnHit);
    PlayEffects();
}

void ASmokeAreaEffect::PlayEffects()
{
    ParticleZoneComponent = UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), Particles, GetActorTransform(), true);
    ParticleZoneComponent->SetRelativeScale3D(FVector(4.f));
    UGameplayStatics::PlaySoundAtLocation(GetWorld(), ExplosionSound, GetActorLocation());
}

// Called every frame
void ASmokeAreaEffect::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

}