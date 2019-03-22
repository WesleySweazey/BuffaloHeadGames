// Fill out your copyright notice in the Description page of Project Settings.

#include "FlashBangTactical.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/SphereComponent.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "Runtime/Engine/Classes/Particles/ParticleSystemComponent.h"
#include "Runtime//Engine/Classes/Sound/SoundCue.h"
#include "Runtime/Engine/Public/WorldCollision.h"
#include "MasterTrappersAlpha1Character.h"

AFlashBangTactical::AFlashBangTactical()
{
    // Use a sphere as a simple collision representation
    CollisionComp = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComp"));
    CollisionComp->InitSphereRadius(5.0f);
    CollisionComp->BodyInstance.SetCollisionProfileName("Projectile");
    CollisionComp->OnComponentHit.AddDynamic(this, &AFlashBangTactical::OnHit);		// set up a notification for when this component hits something blocking

    // Players can't walk on it
    CollisionComp->SetWalkableSlopeOverride(FWalkableSlopeOverride(WalkableSlope_Unwalkable, 0.f));
    CollisionComp->CanCharacterStepUpOn = ECB_No;

    // Set as root component
    RootComponent = CollisionComp;

    StaticMeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    StaticMeshComponent->SetupAttachment(RootComponent);

    // Use a ProjectileMovementComponent to govern this projectile's movement
    ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileComp"));
    ProjectileMovement->UpdatedComponent = CollisionComp;
    ProjectileMovement->InitialSpeed = 1000.0f;
    ProjectileMovement->MaxSpeed = 3000.f;
    ProjectileMovement->bRotationFollowsVelocity = false;
    ProjectileMovement->bShouldBounce = true;

    // Die after 3 seconds by default
    InitialLifeSpan = 3.0f;

    FRotator NewRotation = FRotator(90.0f, 0.0f, 0.0f);

    FQuat QuatRotation = FQuat(NewRotation);
    SetActorRotation(QuatRotation);
    // Initialize the rotate value
    PitchVal = 0.f;
    YawVal = 5.f;
    RollVal = 0.f;
}



void AFlashBangTactical::BeginPlay()
{
    Super::BeginPlay();

    FTimerHandle handle;
    GetWorld()->GetTimerManager().SetTimer(handle, this, &AFlashBangTactical::OnDetonate, 5.f, false);  // don't want to loop the explotion, just do once every 5.0 second
}

void AFlashBangTactical::OnDetonate()
{
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
            AMasterTrappersAlpha1Character* pawn = Cast<AMasterTrappersAlpha1Character>((*Actors).Actor->GetClass());
            if (pawn)
            {
                pawn->StartStun();
            }
        }
    }
    Destroy();
}

void AFlashBangTactical::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
    // Only add impulse and destroy projectile if we hit a physics
    if ((OtherActor != NULL) && (OtherActor != this) && (OtherComp != NULL) /*&& OtherComp->IsSimulatingPhysics()*/)        // I don't care if there's physics happened
    {
        /*OtherComp->AddImpulseAtLocation(GetVelocity() * 100.0f, GetActorLocation());

        Destroy();*/

        //OnDetonate();
    }
    if (OtherActor)
    {
        if (OtherActor->ActorHasTag("Player") || OtherActor->ActorHasTag("AI"))
        {
            AMasterTrappersAlpha1Character* pawn = Cast<AMasterTrappersAlpha1Character>(OtherActor);
            if (pawn)
            {
                GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Blue,
                    "AFlashBangTactical::OnOverlapBegin Overlapped with - "
                    + OtherActor->GetName());
                pawn->StartStun();
                this->Destroy();
            }
        }
    }
}

void AFlashBangTactical::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    Radius += 100.0f;

    // smooth rotating every frame
    FRotator NewRotation = FRotator(PitchVal, YawVal, RollVal);

    FQuat QuatRotation = FQuat(NewRotation);

    AddActorLocalRotation(QuatRotation, false, 0, ETeleportType::None);
}


