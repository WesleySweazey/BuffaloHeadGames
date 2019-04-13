// Fill out your copyright notice in the Description page of Project Settings.

#include "GrenadeTactical.h"
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
#include "Runtime/Engine/Public/TimerManager.h"
#include "Net/UnrealNetwork.h"
AGrenadeTactical::AGrenadeTactical()
{
    // Use a sphere as a simple collision representation
    CollisionComp = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComp"));
    CollisionComp->InitSphereRadius(150.0f);
    CollisionComp->BodyInstance.SetCollisionProfileName("Projectile");
    CollisionComp->OnComponentHit.AddDynamic(this, &AGrenadeTactical::OnHit);		// set up a notification for when this component hits something blocking

    // Players can't walk on it
    CollisionComp->SetWalkableSlopeOverride(FWalkableSlopeOverride(WalkableSlope_Unwalkable, 0.f));
    CollisionComp->CanCharacterStepUpOn = ECB_No;

    // Set as root component
    RootComponent = CollisionComp;

    ExplosionComp = CreateDefaultSubobject<USphereComponent>(TEXT("ExpComp"));
    ExplosionComp->InitSphereRadius(50.0f);
    ExplosionComp->SetCollisionProfileName("OverlapAllDynamic");
    //ExplosionComp->BodyInstance.SetCollisionProfileName("Projectile");
    ExplosionComp->OnComponentBeginOverlap.AddDynamic(this, &AGrenadeTactical::OnOverlapBegin);

    StaticMeshComponent->SetCollisionProfileName("BlockAll");
    StaticMeshComponent->SetupAttachment(RootComponent);

    // Use a ProjectileMovementComponent to govern this projectile's movement
    ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileComp"));
    ProjectileMovement->UpdatedComponent = CollisionComp;
    ProjectileMovement->InitialSpeed = 1000.0f;
    ProjectileMovement->MaxSpeed = 3000.f;
    ProjectileMovement->bRotationFollowsVelocity = false;
    ProjectileMovement->bShouldBounce = true;

    // Die after 3 seconds by default
    Lifetime = 5.0f;
    InitialLifeSpan = 5.2f;

    // Initialize the rotate value
    PitchVal = 5.f;
    YawVal = 5.f;
    RollVal = 5.f;

    SetReplicates(true);
    SetReplicateMovement(true);
}



void AGrenadeTactical::OnExplosion()
{
   /* GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red,
        "OnExplosion ");*/
    TArray<AActor*> FirstFoundActors;
    UGameplayStatics::GetAllActorsOfClass(GetWorld(), AMasterTrappersAlpha1Character::StaticClass(), FirstFoundActors);
    {
        for (int i = 0; i < FirstFoundActors.Num(); i++)
        {
            if (this->GetDistanceTo(FirstFoundActors[i])<150.0f)
            {
                AMasterTrappersAlpha1Character* pawn = Cast<AMasterTrappersAlpha1Character>(FirstFoundActors[i]);

                TArray<AActor*> FoundActors;
                UGameplayStatics::GetAllActorsOfClass(GetWorld(), AMasterTrappersAlpha1Character::StaticClass(), FoundActors);
                for (int j= 0; j < FoundActors.Num(); j++)
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
    }
    //TArray<AActor*> Actors;
    //CollisionComp->GetOverlappingActors(Actors, AMasterTrappersAlpha1Character::StaticClass());
    //for (int i = 0; i < Actors.Num(); i++)
    //{
    //    AMasterTrappersAlpha1Character* pawn = Cast<AMasterTrappersAlpha1Character>(Actors[i]);
    //    if (pawn) //if the flash bang not hitting the player himself
    //    {
    //        TArray<AActor*> FoundActors;
    //        UGameplayStatics::GetAllActorsOfClass(GetWorld(), AMasterTrappersAlpha1Character::StaticClass(), FoundActors);

    //        for (int i = 0; i < FoundActors.Num(); i++)
    //        {
    //            AMasterTrappersAlpha1Character* temp = Cast<AMasterTrappersAlpha1Character>(FoundActors[i]);
    //            if (temp->Team == Team)
    //            {
    //                temp->AddScore();
    //                break;
    //            }
    //        }
    //        pawn->Multicast_Die();
    //    }
    //}

    m_Explosion = UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ExplosionParticles, GetActorTransform());
    if (m_Explosion)
    {
        m_Explosion->SetRelativeScale3D(FVector(4.f));
    }
    UGameplayStatics::PlaySoundAtLocation(GetWorld(), ExplosionSound, GetActorLocation());
}

void AGrenadeTactical::BeginPlay()
{
    Super::BeginPlay();
    UWorld* const World = GetWorld();
    World->GetTimerManager().SetTimer(ExplodeHandle, this, &AGrenadeTactical::OnExplosion, Lifetime, false);
    //FTimerHandle handle;
    //GetWorld()->GetTimerManager().SetTimer(handle, this, &AGrenadeTactical::OnDetonate, 5.f, false);  // don't want to loop the explotion, just do once every 5.0 second
    //GetWorld()->GetTimerManager().SetTimer(Explosionhandle, this, &AGrenadeTactical::OnExplosion, 2.5f, false);
}

void AGrenadeTactical::OnDetonate()
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

            /*if (SM)
            {
                SM->AddRadialImpulse(GetActorLocation(), 1000.0f, 5000.0f, ERadialImpulseFalloff::RIF_Linear, true);
            }*/
            /*else*/ if (DA)
            {
                DA->GetDestructibleComponent()->ApplyRadiusDamage(10.0f, Actors->ImpactPoint, 500.0f, 3000.0f, false);
            }
        }
    }

    Destroy();
}

void AGrenadeTactical::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
    // Only add impulse and destroy projectile if we hit a physics
    //if ((OtherActor != nullptr)/* && (OtherActor != this) && (OtherComp != NULL)*/ /*&& OtherComp->IsSimulatingPhysics()*/)        // I don't care if there's physics happened
    //{
    //    OtherComp->AddImpulseAtLocation(GetVelocity() * 100.0f, GetActorLocation());

    //    Destroy();

    //    OnDetonate();
    //    UParticleSystemComponent* Explosion = UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ExplosionParticles, GetActorTransform());
    //    Explosion->SetRelativeScale3D(FVector(4.f));
    //}
    if (OtherActor)
    {
        if (OtherActor->ActorHasTag("Player"))
        {
            AMasterTrappersAlpha1Character* pawn = Cast<AMasterTrappersAlpha1Character>(OtherActor);
            if (pawn)
            {
                //Checks array of previous collisions
                bool alreadyCollided = false;
                for (int i = 0; i < collidedCharacters.Num(); i++)
                {
                    if (collidedCharacters[i] == pawn)
                    {
                        alreadyCollided = true;
                    }
                }
                if (alreadyCollided == false)
                {
                    collidedCharacters.Add(pawn);
                }
            }
        }
        OnExplosion();
    }
}

void AGrenadeTactical::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    // smooth rotating every frame
    FRotator NewRotation = FRotator(PitchVal, YawVal, RollVal);

    FQuat QuatRotation = FQuat(NewRotation);

    AddActorLocalRotation(QuatRotation, false, 0, ETeleportType::None);
}

void AGrenadeTactical::OnOverlapBegin(UPrimitiveComponent * OverlappedComponent, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
    if (OtherActor)
    {
        if (OtherActor->ActorHasTag("Player") || OtherActor->ActorHasTag("AI"))
        {
            AMasterTrappersAlpha1Character* pawn = Cast<AMasterTrappersAlpha1Character>(OtherActor);
            if (pawn)
            {
                //Checks array of previous collisions
                bool alreadyCollided = false;
                for (int i = 0; i < collidedCharacters.Num(); i++)
                {
                    if (collidedCharacters[i] == pawn)
                    {
                        alreadyCollided = true;
                    }
                }
                if (alreadyCollided == false)
                {
                    collidedCharacters.Add(pawn);
                }
            }
        }
    }
}

//Replicates UPROPERTIES
void AGrenadeTactical::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    DOREPLIFETIME(AGrenadeTactical, m_Explosion);
}
