// Fill out your copyright notice in the Description page of Project Settings.

#include "BaseTactical.h"
#include "MasterTrappersAlpha1Character.h"
#include "Components/StaticMeshComponent.h"

// Sets default values
ABaseTactical::ABaseTactical()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
    StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>("Static Mesh Component");
    RootComponent = StaticMeshComponent;
    Tags.Add("Trap");
}

// Called when the game starts or when spawned
void ABaseTactical::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABaseTactical::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

UStaticMeshComponent * ABaseTactical::GetStaticMeshComponent()
{
    return StaticMeshComponent;
}

