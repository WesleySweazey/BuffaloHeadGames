// Fill out your copyright notice in the Description page of Project Settings.

#include "ItemSpawner.h"
#include "RandomPickup.h"
#include "Components/SceneComponent.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "Components/SphereComponent.h"
#include "MasterTrappersAlpha1Character.h"
#include "Engine/World.h"

// Sets default values
AItemSpawner::AItemSpawner()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
    SceneComponent = CreateDefaultSubobject<USceneComponent>("Main Component");
    RootComponent = SceneComponent;
    SphereComponent = CreateDefaultSubobject<USphereComponent>("Sphere Collision");
    SphereComponent->SetCollisionProfileName("OverlapAllDynamic");
    SphereComponent->OnComponentBeginOverlap.AddDynamic(this,
        &AItemSpawner::OnOverlapBegin);
    SphereComponent->AttachTo(RootComponent);
    SetReplicates(true);
}

// Called when the game starts or when spawned
void AItemSpawner::BeginPlay()
{
	Super::BeginPlay();
    SpawnItem();
}

// Called every frame
void AItemSpawner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AItemSpawner::SpawnItem()
{
    UWorld* const World = GetWorld();
    if (World)
    {
        FActorSpawnParameters SpawnParams;
        SpawnParams.Owner = this;
        SpawnParams.SpawnCollisionHandlingOverride =
            ESpawnActorCollisionHandlingMethod::DontSpawnIfColliding;
        FTransform SpawnTransform = GetActorTransform();
        FRotator SpawnRotation = GetActorRotation();// +FRotator(-90.0f, 0.0f, 0.0f);
        SpawnRotation.Pitch = SpawnRotation.Pitch - 90.0f;

        ARandomPickup* SpawnedActor = World->SpawnActor<ARandomPickup>(RandomPickup, SpawnTransform, SpawnParams);
        SpawnedActor->SetActorRelativeRotation(SpawnRotation.Quaternion());
    }
}

void AItemSpawner::OnOverlapBegin(UPrimitiveComponent * OverlappedComponent, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
    if (OtherActor)
    {
        if (OtherActor->ActorHasTag("Player"))
        {
            AMasterTrappersAlpha1Character* pawn = Cast<AMasterTrappersAlpha1Character>(OtherActor);
            if (pawn)
            {
                UWorld* const World = GetWorld();
                if (World)
                {
                    World->GetTimerManager().SetTimer(SpawnerWait, this, &AItemSpawner::SpawnItem, 3/*rand() % 10 + 20*/, false);
                }
            }
        }
    }
}