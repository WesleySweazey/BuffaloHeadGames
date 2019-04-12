// Fill out your copyright notice in the Description page of Project Settings.




// Fill out your copyright notice in the Description page of Project Settings.

#include "BasePickup.h"
#include "Runtime/Engine/Classes/Components/StaticMeshComponent.h"
#include "Runtime/Engine/Classes/Components/PrimitiveComponent.h"
#include "Runtime/Engine/Classes/Components/BoxComponent.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "Runtime/Engine/Classes/Particles/ParticleSystemComponent.h"
#include "Runtime//Engine/Classes/Sound/SoundCue.h"
#include "MasterTrappersAlpha1Character.h"
#include "Net/UnrealNetwork.h"
#include "Engine/World.h"

// Sets default values
ABasePickup::ABasePickup()
{
    // Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = true;

    RotationRate = FRotator(0.0f, 180.0f, 0.0f);

    ItemMesh = CreateDefaultSubobject<UStaticMeshComponent>("pivkup mesh");
    ItemMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    RootComponent = ItemMesh;

    BoxCollider = CreateDefaultSubobject<UBoxComponent>("pickup box collider");
    BoxCollider->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
    BoxCollider->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
    BoxCollider->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);
    //BoxCollider->bGenerateOverlapEvents = true;
    BoxCollider->SetWorldScale3D(FVector(1.0f, 1.0f, 1.0f));
    //BoxCollider->OnComponentBeginOverlap.AddDynamic(this, &APickupActor::OnOverlapBegin);


    BoxCollider->SetupAttachment(RootComponent);

    PitchValue = 1.f;
    YawValue = 1.f;
    RollValue = 1.f;

    RunningTime = 0.0f;
    SetReplicates(true);
    SetReplicateMovement(true);
}

// Called when the game starts or when spawned
void ABasePickup::BeginPlay()
{
    Super::BeginPlay();

}

// Called every frame
void ABasePickup::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    Rotator(DeltaTime);
}



//void ABasePickup::OnInteract()
//{
//    FString pickup = FString::Printf(TEXT("Picked up: %s"), *Name);
//    GEngine->AddOnScreenDebugMessage(1, 5, FColor::Red, pickup);
//    //Destroy();
//
//    //TODO
//    // Hide item and add it to the inventory
//    // get player first
//    AMasterTrappersAlpha1Character* player = Cast<AMasterTrappersAlpha1Character>(UGameplayStatics::GetPlayerCharacter(this, 0));
//    if (player)
//    {
//        PlayEffects();
//        Show(false);
//        player->AddToInventory(this);
//    }
//
//}

//void ABasePickup::NotifyActorBeginOverlap(AActor * OtherActor)
//{
//    Super::NotifyActorBeginOverlap(OtherActor);
//
//
//
//    //AMasterTrappersAlpha1Character* character = Cast<AMasterTrappersAlpha1Character>(OtherActor);
//    //if (OtherActor == character)
//    //{
//
//    //    //character->bHasKey = true;
//    //    OnInteract();
//    //}
//}

//void APickupActor::OnOverlapBegin(UPrimitiveComponent * OverlappedComp, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
//{
//    // check if not self etc...
//    if ((OtherActor != nullptr) && (OtherActor != this) && (OtherComp != nullptr))
//    {
//        Destroy();
//    }
//}


void ABasePickup::PlayEffects()
{
    /*UParticleSystemComponent* Explosion = UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ExplosionParticles, GetActorTransform());
    Explosion->SetRelativeScale3D(FVector(4.f));*/

    UGameplayStatics::PlaySoundAtLocation(GetWorld(), ExplosionSound, GetActorLocation());
}

void ABasePickup::Show(bool visible)
{
    //set collision type based on visible or not
    ECollisionEnabled::Type collision = visible ? ECollisionEnabled::QueryAndPhysics : ECollisionEnabled::NoCollision;

    // this method decides if this actor's tick() called every frame or not, 
    //if not visible, then not call tick() every frame, to save performance
    SetActorTickEnabled(visible);

    this->ItemMesh->SetVisibility(visible);
    this->ItemMesh->SetCollisionEnabled(collision);

    this->BoxCollider->SetCollisionEnabled(collision);

}

void ABasePickup::Rotator(float DeltaTime)
{
    //Floating part
    FVector NewLocation = GetActorLocation();
    float DeltaHeight = (FMath::Sin(RunningTime + DeltaTime) - FMath::Sin(RunningTime));
    //Scale our height by a factor of 20
    NewLocation.Z += DeltaHeight * 20.0f;
    RunningTime += DeltaTime * 8;
    SetActorLocation(NewLocation);




    FRotator NewRotation = FRotator(PitchValue, YawValue, RollValue);

    FQuat QuatRotation = FQuat(NewRotation);

    AddActorLocalRotation(QuatRotation, false, 0, ETeleportType::None);
}


void ABasePickup::switchTexture()
{
    Image = BlankImage;
}


//Replicates UPROPERTIES
void ABasePickup::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    DOREPLIFETIME(ABasePickup, Name);
    DOREPLIFETIME(ABasePickup, Image);
    DOREPLIFETIME(ABasePickup, BlankImage);
    

}