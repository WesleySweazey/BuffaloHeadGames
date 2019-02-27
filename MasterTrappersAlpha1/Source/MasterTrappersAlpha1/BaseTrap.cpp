// Fill out your copyright notice in the Description page of Project Settings.

#include "BaseTrap.h"
#include "Components/SphereComponent.h"
#include "MasterTrappersAlpha1Character.h"
#include "Components/StaticMeshComponent.h"

// Sets default values
ABaseTrap::ABaseTrap()
{
    // Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = true;
    StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>("Static Mesh Component");
    RootComponent = StaticMeshComponent;
    Tags.Add("Trap");
}

// Called when the game starts or when spawned
void ABaseTrap::BeginPlay()
{
    Super::BeginPlay();
}

// Called every frame
void ABaseTrap::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

}

UStaticMeshComponent * ABaseTrap::GetStaticMeshComponent()
{
    return StaticMeshComponent;
}
