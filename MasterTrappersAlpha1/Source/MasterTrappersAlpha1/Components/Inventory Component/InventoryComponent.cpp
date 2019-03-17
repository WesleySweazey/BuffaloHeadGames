// Fill out your copyright notice in the Description page of Project Settings.

#include "InventoryComponent.h"
#include "Pickups/PickupActor.h"
#include "Net/UnrealNetwork.h" //needed for DOREPLIFETIME
#include "MasterTrappersAlpha1Character.h"
#include "Runtime/Engine/Classes/GameFramework/Actor.h"

// Sets default values for this component's properties

#pragma region Functions
UInventoryComponent::UInventoryComponent()
{
    // Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
    // off to improve performance if you don't need them.
    PrimaryComponentTick.bCanEverTick = true;

    SetIsReplicated(true);
}


// Called when the game starts
void UInventoryComponent::BeginPlay()
{
    Super::BeginPlay();

    CharacterOwner = Cast<AMasterTrappersAlpha1Character>(GetOwner());
    check(CharacterOwner != nullptr && "UInventoryComponent::BeginPlay - CharacterOwner is Null");

}


// Called every frame
void UInventoryComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UInventoryComponent::AddToInventory(class APickupActor* pickup)
{
    Inventory.AddUnique(pickup);
    pickup->Disable();
}

class APickupActor* UInventoryComponent::RemoveCurrentInventoryItem()
{
    int32 Index = 0;

    if (CurrentInventory)
    {

        if (Inventory.Find(CurrentInventory, Index))
        {
            Inventory.RemoveAt(Index, 1, true);

            CurrentInventory->SetActorLocation(GetOwner()->GetActorLocation());
            CurrentInventory->Enable();
            ResetCurrentInventory();
            return CurrentInventory;
        }
    }
    return nullptr;
}


class APickupActor* UInventoryComponent::GetCurrentInventory() const
{
    return CurrentInventory;
}

void UInventoryComponent::ResetCurrentInventory()
{
    CurrentInventory = nullptr;
}

int UInventoryComponent::GetInventoryCount()
{
    return Inventory.Num();
}

#pragma endregion Normal Functions

#pragma region Network Functions

void UInventoryComponent::NextInventoryItem_Implementation()
{
    //No items means there is nothing to equip
    if (Inventory.Num() == 0)
    {
        return;
    }

    //Get the index of the current item and increment it
    APickupActor* NewInventory = nullptr;
    int32 Index = 0;
    if (CurrentInventory)
    {
        Inventory.Find(CurrentInventory, Index);
        Index += 1;
    }

    //Set new item when index is not overflowing
    if (Index < Inventory.Num())
    {
        NewInventory = Inventory[Index];
    }

    CurrentInventory = NewInventory;
}

bool UInventoryComponent::NextInventoryItem_Validate()
{
    return true;
}


void UInventoryComponent::PreviousInventoryItem_Implementation()
{
    //No items means there is nothing to equip
    if (Inventory.Num() == 0)
    {
        return;
    }

    //Get the index of the current item and decrement it
    APickupActor* NewInventory = nullptr;
    int32 Index = Inventory.Num() - 1;
    if (CurrentInventory)
    {
        Inventory.Find(CurrentInventory, Index);
        Index -= 1;
    }

    //Set new item when index is inside array
    if (Index > -1)
    {
        NewInventory = Inventory[Index];
    }
    CurrentInventory = NewInventory;
}

bool UInventoryComponent::PreviousInventoryItem_Validate()
{
    return true;
}

void UInventoryComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty> & OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    DOREPLIFETIME(UInventoryComponent, Inventory);
    DOREPLIFETIME(UInventoryComponent, CurrentInventory);

}

#pragma endregion Functions that would need Networking

