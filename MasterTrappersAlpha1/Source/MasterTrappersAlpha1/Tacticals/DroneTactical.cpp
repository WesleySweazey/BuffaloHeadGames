// Fill out your copyright notice in the Description page of Project Settings.

#include "DroneTactical.h"
#include "Components/SphereComponent.h"
#include "Components/AudioComponent.h"
#include "Engine/Engine.h"
#include "MasterTrappersAlpha1Character.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "Runtime/Engine/Classes/Particles/ParticleSystemComponent.h"
#include "Runtime//Engine/Classes/Sound/SoundCue.h"
#include "Runtime/Engine/Public/WorldCollision.h"
#include "Runtime/CoreUObject/Public/UObject/ConstructorHelpers.h"


ADroneTactical::ADroneTactical()
{
    PrimaryActorTick.bCanEverTick = true;
    OverlapComponent = CreateDefaultSubobject<USphereComponent>("OverlapSphere");
    OverlapComponent->SetGenerateOverlapEvents(true);
    OverlapComponent->BodyInstance.SetCollisionProfileName("Trigger");//OverlapAllDynamic
    OverlapComponent->SetNotifyRigidBodyCollision(false);
    OverlapComponent->OnComponentBeginOverlap.AddDynamic(this, &ADroneTactical::StartOverlap);
    OverlapComponent->OnComponentEndOverlap.AddDynamic(this, &ADroneTactical::EndOverlap);

    RootComponent = OverlapComponent;

    StaticMeshComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
    StaticMeshComponent->SetupAttachment(RootComponent);


    static ConstructorHelpers::FObjectFinder<USoundCue> TargetSpottedSound(TEXT("SoundCue'/Game/StarterContent/Audio/Steam01_Cue.Steam01_Cue'"));
    if (TargetSpottedSound.Succeeded())
    {
        TargetSpottedCue = TargetSpottedSound.Object;
        DroneAudio = CreateDefaultSubobject<UAudioComponent>("Drone Audio");
        DroneAudio->SetupAttachment(RootComponent);
        DroneAudio->SetSound(TargetSpottedCue);
    }
}

void ADroneTactical::BeginPlay()
{
    Super::BeginPlay();

}

void ADroneTactical::StartOverlap(UPrimitiveComponent * OverlappedComponent, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
    if (OtherActor->ActorHasTag("Player"))
    {
        GEngine->AddOnScreenDebugMessage(-1, 3, FColor::Red, "Overlap Start");
        DroneAudio->Play();

    }
}

void ADroneTactical::EndOverlap(UPrimitiveComponent * OverlappedComp, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex)
{
    if (OtherActor->ActorHasTag("Player"))
    {
        GEngine->AddOnScreenDebugMessage(-1, 3, FColor::Red, "Overlap End");
        DroneAudio->Stop();

    }
}

void ADroneTactical::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

}
