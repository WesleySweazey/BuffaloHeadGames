// Fill out your copyright notice in the Description page of Project Settings.

#include "HealthComponent.h"
#include "Net/UnrealNetwork.h"

// Sets default values for this component's properties
UHealthComponent::UHealthComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

    bCanBeDamaged = true;
    FullHealth = 1000.f;
    Health = FullHealth;
    HealthPercentage = 1.0f;
	// ...

    SetIsReplicated(true);
}


// Called when the game starts
void UHealthComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UHealthComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

float UHealthComponent::TakeDamage(float DamageAmount)
{
    bCanBeDamaged = false;
    UpdateHealth(-DamageAmount);
    return DamageAmount;
}

float UHealthComponent::GetPercentageHealth()
{
    return HealthPercentage;
}

float UHealthComponent::GetHealth()
{
    return Health;
}

void UHealthComponent::UpdateHealth(float HealthChange)
{
    Health += HealthChange;
    Health = FMath::Clamp(Health, 0.0f, FullHealth);
    HealthPercentage = Health / FullHealth;
}

bool UHealthComponent::Server_ResetHealth_Validate()
{
    return true;
}

void UHealthComponent::Server_ResetHealth_Implementation()
{
    Health = FullHealth;
}

//Replicates UPROPERTIES
void UHealthComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    DOREPLIFETIME(UHealthComponent, Health);
}