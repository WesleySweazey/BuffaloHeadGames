// Fill out your copyright notice in the Description page of Project Settings.

#include "BaseTrap.h"
#include "Components/SphereComponent.h"
#include "MasterTrappersAlpha1Character.h"
#include "Components/StaticMeshComponent.h"
#include "Net/UnrealNetwork.h"

// Sets default values
ABaseTrap::ABaseTrap()
{
    // Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = true;
    StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>("Static Mesh Component");
    RootComponent = StaticMeshComponent;
    SetReplicates(true);
    Tags.Add("Trap");
}

// Called when the game starts or when spawned
void ABaseTrap::BeginPlay()
{
    Super::BeginPlay();
    StaticMeshComponent->SetMaterial(0, TrapMaterial); 
    Server_SetMaterial(TrapMaterial);
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

bool ABaseTrap::Server_SetMaterial_Validate(UMaterialInterface* mat)
{
    return true;
}

void ABaseTrap::Server_SetMaterial_Implementation(UMaterialInterface* mat)
{
    StaticMeshComponent->SetMaterial(0, mat);
}

//void ABaseTrap::Multicast_Destory_Implementation()
//{
//
//}


void ABaseTrap::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    DOREPLIFETIME(ABaseTrap, Team);
    DOREPLIFETIME(ABaseTrap, TrapMaterial);
}