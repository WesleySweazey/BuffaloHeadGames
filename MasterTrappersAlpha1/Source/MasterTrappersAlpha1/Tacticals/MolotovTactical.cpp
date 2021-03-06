// Fill out your copyright notice in the Description page of Project Settings.

#include "MolotovTactical.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/SphereComponent.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "Runtime/Engine/Classes/Particles/ParticleSystemComponent.h"
#include "Runtime//Engine/Classes/Sound/SoundCue.h"
#include "Runtime/Engine/Public/WorldCollision.h"
#include "MasterTrappersAlpha1Character.h"
#include "AreaEffects/FireAreaEffect.h"
#include "Engine/World.h"
#include "AreaEffects/BaseAreaEffect.h"
#include "Runtime/Engine/Classes/Components/StaticMeshComponent.h"
#include "Runtime/Engine/Public/TimerManager.h"
#include "Engine/Engine.h"

AMolotovTactical::AMolotovTactical()
{
    // Use a sphere as a simple collision representation
    //CollisionComp = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComp"));
    //CollisionComp->InitSphereRadius(5.0f);
    //CollisionComp->BodyInstance.SetCollisionProfileName("Projectile");
    //CollisionComp->OnComponentHit.AddDynamic(this, &AMolotovTactical::OnHit);		// set up a notification for when this component hits something blocking

    //// Players can't walk on it
    //CollisionComp->SetWalkableSlopeOverride(FWalkableSlopeOverride(WalkableSlope_Unwalkable, 0.f));
    //CollisionComp->CanCharacterStepUpOn = ECB_No;

    //// Set as root component
    //RootComponent = CollisionComp;

    //StaticMeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    //StaticMeshComponent->SetupAttachment(RootComponent);

    ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileComp"));
    ProjectileMovement->UpdatedComponent = StaticMeshComponent;
    ProjectileMovement->InitialSpeed = 1000.0f;
    ProjectileMovement->MaxSpeed = 1000.f;
    //ProjectileMovement->SetVelocityInLocalSpace(FowardVelocity);
    ProjectileMovement->bRotationFollowsVelocity = false;
    ProjectileMovement->bShouldBounce = true;

    GetStaticMeshComponent()->OnComponentHit.AddDynamic(this, &AMolotovTactical::Server_OnHit);

    // Die after 3 seconds by default
    InitialLifeSpan = 6.0f;

    /*FRotator NewRotation = FRotator(90.0f, 0.0f, 0.0f);

    FQuat QuatRotation = FQuat(NewRotation);
    SetActorRotation(QuatRotation);*/
    // Initialize the rotate value
    //PitchVal = 5.0f;
    //YawVal = 5.f;
    //RollVal = 5.f;

    SetReplicates(true);
    SetReplicateMovement(true);
}



void AMolotovTactical::BeginPlay()
{
    Super::BeginPlay();

    FTimerHandle handle;
   // GetWorld()->GetTimerManager().SetTimer(handle, this, &AMolotovTactical::OnDetonate, 5.f, false);  // don't want to loop the explotion, just do once every 5.0 second
}

void AMolotovTactical::OnDetonate()
{
    //Spawns Fire
    UWorld* const World = GetWorld();
    FActorSpawnParameters SpawnParams;
    SpawnParams.Owner = this;
    SpawnParams.SpawnCollisionHandlingOverride =
        ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
    FTransform SpawnTransform = GetActorTransform();
    FRotator SpawnRotation = GetActorRotation();// +FRotator(-90.0f, 0.0f, 0.0f);
    SpawnRotation.Pitch = SpawnRotation.Pitch - 90.0f;
    AFireAreaEffect* SpawnedActor = World->SpawnActor<AFireAreaEffect>(FireAreaEffect, SpawnTransform, SpawnParams);
    SpawnedActor->SetActorRelativeRotation(SpawnRotation.Quaternion());
    SpawnedActor->SetTeam(Team);
    if (SpawnedActor)
    {
        UE_LOG(LogTemp, Warning, TEXT("Fire Area Effect Spawned"));
    }

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
    //Destroy();
}

bool AMolotovTactical::Server_OnHit_Validate(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
    return true;
}

void AMolotovTactical::Server_OnHit_Implementation(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
    // Only add impulse and destroy projectile if we hit a physics
    if ((OtherActor != NULL) && (OtherActor != this) && (OtherComp != NULL) && OtherActor != this->GetOwner())        // I don't care if there's physics happened
    {
        GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Green, ("Molotov hit "));
        UWorld* const World = GetWorld();
        FActorSpawnParameters SpawnParams;
        SpawnParams.Owner = this;
        SpawnParams.SpawnCollisionHandlingOverride =
            ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
        FTransform SpawnTransform = GetActorTransform();
        FRotator SpawnRotation = GetActorRotation();// +FRotator(-90.0f, 0.0f, 0.0f);
        SpawnRotation.Pitch = SpawnRotation.Pitch - 90.0f;
        AFireAreaEffect* SpawnedActor = World->SpawnActor<AFireAreaEffect>(FireAreaEffect, SpawnTransform, SpawnParams);
        SpawnedActor->SetActorRelativeRotation(SpawnRotation.Quaternion());
        SpawnedActor->SetTeam(Team);
        if (SpawnedActor)
        {
            UE_LOG(LogTemp, Warning, TEXT("Fire Area Effect Spawned"));
        }

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
        Destroy();
        /*OtherComp->AddImpulseAtLocation(GetVelocity() * 100.0f, GetActorLocation());

        */

        //OnDetonate();
    }
    /*if (OtherActor)
    {
        OnDetonate();
    }*/
    
}

void AMolotovTactical::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    // smooth rotating every frame
    /*FRotator NewRotation = FRotator(PitchVal, YawVal, RollVal);

    FQuat QuatRotation = FQuat(NewRotation);

    AddActorLocalRotation(QuatRotation, false, 0, ETeleportType::None);*/
}


