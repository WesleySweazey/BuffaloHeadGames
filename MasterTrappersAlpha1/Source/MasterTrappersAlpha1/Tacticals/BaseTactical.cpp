// Fill out your copyright notice in the Description page of Project Settings.

#include "BaseTactical.h"
#include "MasterTrappersAlpha1Character.h"
#include "Components/StaticMeshComponent.h"
#include "Net/UnrealNetwork.h"
#include "Engine/Engine.h"
// Sets default values
ABaseTactical::ABaseTactical()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
    StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>("Static Mesh Component");
    RootComponent = StaticMeshComponent;
    SetReplicates(true);
    SetReplicateMovement(true);
    Tags.Add("Tactical");
}

// Called when the game starts or when spawned
void ABaseTactical::BeginPlay()
{
	Super::BeginPlay();
    StaticMeshComponent->SetMaterial(0, TacticalMaterial);
}

// Called every frame
void ABaseTactical::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

bool ABaseTactical::Server_SetMaterial_Validate(UMaterialInterface* mat)
{
    return true;
}

void ABaseTactical::Server_SetMaterial_Implementation(UMaterialInterface* mat)
{
    StaticMeshComponent->SetMaterial(0, mat);
}

UStaticMeshComponent * ABaseTactical::GetStaticMeshComponent()
{
    return StaticMeshComponent;
}

void ABaseTactical::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    DOREPLIFETIME(ABaseTactical, Team);
    DOREPLIFETIME(ABaseTactical, TacticalMaterial);
}