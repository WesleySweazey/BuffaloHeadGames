// Fill out your copyright notice in the Description page of Project Settings.

#include "FlashBangTactical.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/SphereComponent.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "Runtime/Engine/Classes/Particles/ParticleSystemComponent.h"
#include "Runtime//Engine/Classes/Sound/SoundCue.h"
#include "Runtime/Engine/Public/WorldCollision.h"
#include "MasterTrappersAlpha1Character.h"
#include "AreaEffects/SmokeAreaEffect.h"
#include "Engine/World.h"
#include "Runtime/Engine/Classes/Components/StaticMeshComponent.h"
#include "Runtime/Engine/Public/TimerManager.h"

AFlashBangTactical::AFlashBangTactical()
{
    // Use a sphere as a simple collision representation
    //CollisionComp = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComp"));
    //CollisionComp->InitSphereRadius(500.0f);
    //CollisionComp->BodyInstance.SetCollisionProfileName("Projectile");
    //CollisionComp->OnComponentHit.AddDynamic(this, &AFlashBangTactical::OnHit);		// set up a notification for when this component hits something blocking

    //// Players can't walk on it
    //CollisionComp->SetWalkableSlopeOverride(FWalkableSlopeOverride(WalkableSlope_Unwalkable, 0.f));
    //CollisionComp->CanCharacterStepUpOn = ECB_No;

    //// Set as root component
    //RootComponent = CollisionComp;

    

    //StaticMeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    //StaticMeshComponent->SetupAttachment(RootComponent);

    // Use a ProjectileMovementComponent to govern this projectile's movement
    ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileComp"));
    ProjectileMovement->UpdatedComponent = StaticMeshComponent;
    ProjectileMovement->InitialSpeed = 1000.0f;
    ProjectileMovement->MaxSpeed = 3000.f;
    ProjectileMovement->bRotationFollowsVelocity = false;
    ProjectileMovement->bShouldBounce = true;

    ExplosionComp = CreateDefaultSubobject<USphereComponent>(TEXT("ExplosionComp"));
    ExplosionComp->InitSphereRadius(2000.0f);
    ExplosionComp->SetCollisionProfileName("OverlapAllDynamic");
    //ExplosionComp->OnComponentHit.AddDynamic(this, &AFlashBangTactical::OnHit);
    ExplosionComp->SetupAttachment(RootComponent);

    // Die after 3 seconds by default
    InitialLifeSpan = 6.0f;

    FRotator NewRotation = FRotator(90.0f, 0.0f, 0.0f);

    FQuat QuatRotation = FQuat(NewRotation);
    SetActorRotation(QuatRotation);
    // Initialize the rotate value
    PitchVal = 0.f;
    YawVal = 5.f;
    RollVal = 0.f;

    SetReplicates(true);
    SetReplicateMovement(true);
}

AFlashBangTactical::~AFlashBangTactical()
{
    //m_Explosion->bAutoDestroy = true;
}

void AFlashBangTactical::StartSmoke()
{
        UWorld* const World = GetWorld();
        FActorSpawnParameters SpawnParams;
        SpawnParams.Owner = this;
        SpawnParams.SpawnCollisionHandlingOverride =
            ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
        FTransform SpawnTransform = GetActorTransform();
        FRotator SpawnRotation = GetActorRotation();// +FRotator(-90.0f, 0.0f, 0.0f);
        SpawnRotation.Pitch = SpawnRotation.Pitch - 90.0f;
        ASmokeAreaEffect* SpawnedActor = World->SpawnActor<ASmokeAreaEffect>(SmokeEffect, SpawnTransform, SpawnParams);
        SpawnedActor->SetActorRelativeRotation(SpawnRotation.Quaternion());
        SpawnedActor->SetTeam(Team);
        if (SpawnedActor)
        {
            UE_LOG(LogTemp, Warning, TEXT("Smoke Area Effect Spawned"));
        }
}

void AFlashBangTactical::BeginPlay()
{
    Super::BeginPlay();

    FTimerHandle handle;
    GetWorld()->GetTimerManager().SetTimer(handle, this, &AFlashBangTactical::OnDetonate, 5.f, false);  // don't want to loop the explotion, just do once every 5.0 second
    GetWorld()->GetTimerManager().SetTimer(smokeTimerHandle, this, &AFlashBangTactical::StartSmoke, 2.0f, false);                                        //Spawns Smoke
}

void AFlashBangTactical::OnDetonate()
{
    UGameplayStatics::PlaySoundAtLocation(GetWorld(), ExplosionSound, GetActorLocation());
    UParticleSystemComponent* Explosion = UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ExplosionParticles, GetActorTransform());
    Explosion->SetRelativeScale3D(FVector(4.f));
    Explosion->SetIsReplicated(true);
    //TArray<FHitResult> HitActors;
    TArray<AActor*> Actors;
    ExplosionComp->GetOverlappingActors(Actors, AMasterTrappersAlpha1Character::StaticClass());
    for (int i = 0; i < Actors.Num(); i++)
    {
        AMasterTrappersAlpha1Character* pawn = Cast<AMasterTrappersAlpha1Character>(Actors[i]);
        if (pawn) //if the flash bang not hitting the player himself
        {
            /*GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Blue,
            "AFlashBangTactical::OnOverlapBegin Overlapped with - "
            + OtherActor->GetName());*/
            if (Role < ROLE_Authority)
            {
                pawn->Server_StartStun();
            }
            else
            {
                pawn->Client_StartStun();
            }
        }
    }
    /*FVector StartTrace = GetActorLocation();
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
                pawn->Server_StartStun();
            }
        }
    }
    Destroy();*/
}

void AFlashBangTactical::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
    
    // Only add impulse and destroy projectile if we hit a physics
    //if ((OtherActor != NULL) && (OtherActor != this) && (OtherComp != NULL) /*&& OtherComp->IsSimulatingPhysics()*/)        // I don't care if there's physics happened
    //{
    //    /*OtherComp->AddImpulseAtLocation(GetVelocity() * 100.0f, GetActorLocation());

    //    Destroy();*/

    //    //OnDetonate();
    //}
    //if (OtherActor)
    //{
    //    if (OtherActor->ActorHasTag("Player") || OtherActor->ActorHasTag("AI"))
    //    {
    //        AMasterTrappersAlpha1Character* pawn = Cast<AMasterTrappersAlpha1Character>(OtherActor);
    //        if (pawn != GetOwner() && pawn->Team != Team) //if the flash bang not hitting the player himself
    //        {
    //            /*GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Blue,
    //                "AFlashBangTactical::OnOverlapBegin Overlapped with - "
    //                + OtherActor->GetName());*/
    //            if (Role == ROLE_Authority)
    //            {
    //                pawn->Server_StartStun();
    //            }
    //            else
    //            {
    //                pawn->Client_StartStun();
    //            }
    //            this->Destroy();
    //        }
    //    }
    //}
}

void AFlashBangTactical::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    //Radius += 100.0f;

    // smooth rotating every frame
    FRotator NewRotation = FRotator(PitchVal, YawVal, RollVal);

    FQuat QuatRotation = FQuat(NewRotation);

    AddActorLocalRotation(QuatRotation, false, 0, ETeleportType::None);
    //ProjectileMovement->UpdateComponentVelocity();
}


