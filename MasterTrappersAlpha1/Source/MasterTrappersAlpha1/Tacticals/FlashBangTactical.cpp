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
#include "Engine.h"
#include "Net/UnrealNetwork.h"
//#include "Runtime/Engine/Public/EngineGlobals.h"

AFlashBangTactical::AFlashBangTactical()
{
    // Use a ProjectileMovementComponent to govern this projectile's movement
    ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileComp"));
    ProjectileMovement->UpdatedComponent = StaticMeshComponent;
    ProjectileMovement->InitialSpeed = 1000.0f;
    ProjectileMovement->MaxSpeed = 1000.f;
    ProjectileMovement->bRotationFollowsVelocity = false;
    ProjectileMovement->bShouldBounce = true;

    ExplosionComp = CreateDefaultSubobject<USphereComponent>(TEXT("ExplosionComp"));
    ExplosionComp->InitSphereRadius(10.0f);
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

void AFlashBangTactical::StartFlash()
{
    UGameplayStatics::PlaySoundAtLocation(GetWorld(), ExplosionSound, GetActorLocation());
    UParticleSystemComponent* Explosion = UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ExplosionParticles, GetActorTransform());
    Explosion->SetRelativeScale3D(FVector(4.f));
    Explosion->SetIsReplicated(true);
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
    if (Role == ROLE_Authority)
    {
        GetWorld()->GetTimerManager().SetTimer(handle, this, &AFlashBangTactical::Server_OnDetonate_Implementation, 5.f, false);  // don't want to loop the explotion, just do once every 5.0 second
    }
    GetWorld()->GetTimerManager().SetTimer(smokeTimerHandle, this, &AFlashBangTactical::StartSmoke, 4.0f, false);  //Spawns Smoke
    GetWorld()->GetTimerManager().SetTimer(smokeTimerHandle, this, &AFlashBangTactical::StartFlash, 5.0f, false);  //Spawns flash
}

bool AFlashBangTactical::Server_OnDetonate_Validate()
{
    return true;
}

void AFlashBangTactical::Server_OnDetonate_Implementation()
{
    

    TArray<AActor*> FirstFoundActors;
    UGameplayStatics::GetAllActorsOfClass(GetWorld(), AMasterTrappersAlpha1Character::StaticClass(), FirstFoundActors);
    {
        GEngine->AddOnScreenDebugMessage(-1, 30.0f, FColor::Yellow, "Num of actors: " + FString::SanitizeFloat(FirstFoundActors.Num()));
        for (int i = 0; i < FirstFoundActors.Num(); i++)
        {
            AMasterTrappersAlpha1Character* pawn = Cast<AMasterTrappersAlpha1Character>(FirstFoundActors[i]);
            FVector pawnPos = pawn->SavedPosition;
            //FVector::Distance(this->GetActorLocation(), pawnPos);
            //this->GetDistanceTo(pawnPos)
            float dist = FVector::Distance(this->GetActorLocation(), pawnPos);
            //GEngine->AddOnScreenDebugMessage(-1, 150.0f, FColor::Red, pawn->GetName() + "Distance: " + FString::SanitizeFloat(FVector::Distance(this->GetActorLocation(), pawnPos)));
            //GEngine->AddOnScreenDebugMessage(-1, 150.0f, FColor::Red, pawn->GetName() + " Flash position: X" + FString::SanitizeFloat(this->GetActorLocation().X) + " Actor position: Y" + FString::SanitizeFloat(this->GetActorLocation().Y) + " Actor position: Z" + FString::SanitizeFloat(this->GetActorLocation().Z));
            if (dist < 500.0f)
            {
                
                //GEngine->AddOnScreenDebugMessage(-1, 45.0f, FColor::Red, pawn->GetName() + " Actor position: X" + FString::SanitizeFloat(pawn->GetActorLocation().X) + " Actor position: Y" + FString::SanitizeFloat(pawn->GetActorLocation().Y) + " Actor position: Z" + FString::SanitizeFloat(pawn->GetActorLocation().Z));
                GEngine->AddOnScreenDebugMessage(-1, 150.0f, FColor::Red, pawn->GetName() + " Actor position: X" + FString::SanitizeFloat(pawn->SavedPosition.X) + " Actor position: Y" + FString::SanitizeFloat(pawn->SavedPosition.Y) + " Actor position: Z" + FString::SanitizeFloat(pawn->SavedPosition.Z));
                //GEngine->AddOnScreenDebugMessage(-1, 150.0f, FColor::Red, pawn->GetName() + " Flash position: X" + FString::SanitizeFloat(this->GetActorLocation().X) + " Actor position: Y" + FString::SanitizeFloat(this->GetActorLocation().Y) + " Actor position: Z" + FString::SanitizeFloat(this->GetActorLocation().Z));
                
                //pawn->GetMesh1P();
                TArray<AActor*> FoundActors;
                UGameplayStatics::GetAllActorsOfClass(GetWorld(), AMasterTrappersAlpha1Character::StaticClass(), FoundActors);
                for (int j = 0; j < FoundActors.Num(); j++)
                {
                    AMasterTrappersAlpha1Character* temp = Cast<AMasterTrappersAlpha1Character>(FoundActors[j]);
                    if (temp->Team == Team)
                    {
                        temp->AddScore();
                        break;
                    }
                }
                //pawn->Multicast_Die();
                GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Blue, "AFlashBangTactical::Start flash");
                pawn->Client_StartStun();
            }
        }
    }
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


