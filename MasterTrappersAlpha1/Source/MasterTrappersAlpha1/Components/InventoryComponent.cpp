// Fill out your copyright notice in the Description page of Project Settings.

#include "InventoryComponent.h"
#include "Pickups/BasePickup.h"

// Sets default values for this component's properties
UInventoryComponent::UInventoryComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UInventoryComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UInventoryComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UInventoryComponent::AddToTacticalsInventory(ABasePickup * actor)
{
    _inventory_tacticals.Add(actor);

}

void UInventoryComponent::UpdateTacticalsInventory()
{
    FString sInventory = "";
    // for each inventory item
    for (ABasePickup* actor : _inventory_tacticals)
    {
        sInventory.Append(actor->Name);
        sInventory.Append(" | ");
    }

    GEngine->AddOnScreenDebugMessage(1, 3, FColor::White, *sInventory);
    //OnUpdateInventory.Broadcast(_inventory);
}

void UInventoryComponent::RemoveFromTacticalInventory()
{
    //ABasePickup*& LastActor = _inventory_tacticals.Last();
    _inventory_tacticals.Pop();
}