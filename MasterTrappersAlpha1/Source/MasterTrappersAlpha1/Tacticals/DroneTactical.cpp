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
#include "Runtime/Engine/Public/DrawDebugHelpers.h"


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

    //create and set drone audio for spotting targets
    static ConstructorHelpers::FObjectFinder<USoundCue> TargetSpottedSound(TEXT("SoundCue'/Game/StarterContent/Audio/Steam01_Cue.Steam01_Cue'"));
    //if the sound cue was found then proceed with creating  and setting drone audio
    if (TargetSpottedSound.Succeeded())
    {
        TargetSpottedCue = TargetSpottedSound.Object;
        DroneAudio = CreateDefaultSubobject<UAudioComponent>("Drone Audio");
        DroneAudio->SetupAttachment(RootComponent);
        DroneAudio->SetSound(TargetSpottedCue);
    }

    SetReplicates(true);
    SetReplicateMovement(true);

    DroneDir = FVector(4, 0, 0);
}

void ADroneTactical::BeginPlay()
{
    Super::BeginPlay();
    //stops sound from playing right away
    DroneAudio->Stop();

}

void ADroneTactical::StartOverlap(UPrimitiveComponent * OverlappedComponent, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
    if (OtherActor->ActorHasTag("Player"))
    {
        GEngine->AddOnScreenDebugMessage(-1, 3, FColor::Red, "Overlap Start");
        //plays sound when players enter radius
        DroneAudio->Play();
         
        
    }
}

void ADroneTactical::EndOverlap(UPrimitiveComponent * OverlappedComp, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex)
{
    if (OtherActor->ActorHasTag("Player"))
    {
        GEngine->AddOnScreenDebugMessage(-1, 3, FColor::Red, "Overlap End");
        //stops playing sound when players leave drone radius 
        DroneAudio->Stop();

    }
}

void ADroneTactical::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    //constantly check if there is a wall blocking path
    CheckForWalls();

    SetActorLocation(GetActorLocation() += DroneDir);
    
}

void ADroneTactical::CheckForWalls()
{
    FHitResult* HitResult = new FHitResult();
    FVector RayStartPoint = GetActorLocation();
    FVector RayEndPoint = RayStartPoint + (FVector::ForwardVector * 20);
    FCollisionQueryParams* CQP = new FCollisionQueryParams();

    /*!HitResult->GetActor()->ActorHasTag("Player")|| !HitResult->GetActor()->ActorHasTag("Tactical") || !HitResult->GetActor()->ActorHasTag("Trap")*/

    if (GetWorld()->LineTraceSingleByChannel(*HitResult, RayStartPoint, RayEndPoint, ECC_Visibility, *CQP))
    {
        DrawDebugLine(GetWorld(), RayStartPoint, RayEndPoint, FColor(255, 0, 0), true);


        int rand = FMath::RandRange(0, 2);
        if (rand == 0)
        {
            SetActorRotation(GetActorRotation() + FRotator(0, 90, 0));
            DroneDir = FVector(0, 4, 0);
            RayEndPoint = RayStartPoint + (FVector::RightVector * 20);
        }
        if (rand == 1)
        {
            SetActorRotation(GetActorRotation() + FRotator(0, 180, 0));
            DroneDir = FVector(-4, 0, 0);
            RayEndPoint = RayStartPoint + (-FVector::ForwardVector * 20);
        }
        if (rand == 2)
        {
            SetActorRotation(GetActorRotation() + FRotator(0, 270, 0));
            DroneDir = FVector(0, -4, 0);
            RayEndPoint = RayStartPoint + (-FVector::RightVector * 20);
        }
    }
}
