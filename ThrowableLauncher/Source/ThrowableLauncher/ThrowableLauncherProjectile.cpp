// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#include "ThrowableLauncherProjectile.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/SphereComponent.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "Runtime/Engine/Classes/Particles/ParticleSystemComponent.h"
#include "Runtime//Engine/Classes/Sound/SoundCue.h"
#include "Runtime/Engine/Public/WorldCollision.h"
#include "DestructibleActor.h"
#include "DestructibleComponent.h"

AThrowableLauncherProjectile::AThrowableLauncherProjectile() 
{
	// Use a sphere as a simple collision representation
	CollisionComp = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComp"));
	CollisionComp->InitSphereRadius(5.0f);
	CollisionComp->BodyInstance.SetCollisionProfileName("Projectile");
	CollisionComp->OnComponentHit.AddDynamic(this, &AThrowableLauncherProjectile::OnHit);		// set up a notification for when this component hits something blocking

	// Players can't walk on it
	CollisionComp->SetWalkableSlopeOverride(FWalkableSlopeOverride(WalkableSlope_Unwalkable, 0.f));
	CollisionComp->CanCharacterStepUpOn = ECB_No;

	// Set as root component
	RootComponent = CollisionComp;

	// Use a ProjectileMovementComponent to govern this projectile's movement
	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileComp"));
	ProjectileMovement->UpdatedComponent = CollisionComp;
	ProjectileMovement->InitialSpeed = 1000.0f;
	ProjectileMovement->MaxSpeed = 3000.f;
    ProjectileMovement->bRotationFollowsVelocity = false;
	ProjectileMovement->bShouldBounce = true;

	// Die after 3 seconds by default
	InitialLifeSpan = 3.0f;

    // Initialize the rotate value
    PitchVal = 5.f;
    YawVal = 5.f;
    RollVal = 5.f;
}



void AThrowableLauncherProjectile::BeginPlay()
{
    Super::BeginPlay();

    FTimerHandle handle;
    GetWorld()->GetTimerManager().SetTimer(handle, this, &AThrowableLauncherProjectile::OnDetonate, 5.f, false);  // don't want to loop the explotion, just do once every 5.0 second
}

void AThrowableLauncherProjectile::OnDetonate()
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
            UStaticMeshComponent* SM = Cast<UStaticMeshComponent>((*Actors).Actor->GetRootComponent());
            ADestructibleActor* DA = Cast<ADestructibleActor>((*Actors).GetActor());

            if (SM)
            {
                SM->AddRadialImpulse(GetActorLocation(), 1000.0f, 5000.0f, ERadialImpulseFalloff::RIF_Linear, true);
            }
            else if (DA)
            {
                DA->GetDestructibleComponent()->ApplyRadiusDamage(10.0f, Actors->ImpactPoint, 500.0f, 3000.0f, false);
            }
        }
    }

    Destroy();
}

void AThrowableLauncherProjectile::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	// Only add impulse and destroy projectile if we hit a physics
	if ((OtherActor != NULL) && (OtherActor != this) && (OtherComp != NULL) /*&& OtherComp->IsSimulatingPhysics()*/ )        // I don't care if there's physics happened
	{
		/*OtherComp->AddImpulseAtLocation(GetVelocity() * 100.0f, GetActorLocation());

		Destroy();*/

        OnDetonate();
	}
}

void AThrowableLauncherProjectile::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    // smooth rotating every frame
    FRotator NewRotation = FRotator(PitchVal, YawVal, RollVal);

    FQuat QuatRotation = FQuat(NewRotation);

    AddActorLocalRotation(QuatRotation, false, 0, ETeleportType::None);
}
