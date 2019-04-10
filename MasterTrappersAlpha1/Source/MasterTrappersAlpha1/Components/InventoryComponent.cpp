// Fill out your copyright notice in the Description page of Project Settings.

#include "InventoryComponent.h"
#include "Pickups/BasePickup.h"
#include "Runtime/Engine/Classes/Components/SceneComponent.h"
#include "MasterTrappersAlpha1Character.h"
#include "Net/UnrealNetwork.h"



// Sets default values for this component's properties
UInventoryComponent::UInventoryComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
    SetIsReplicated(true);
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


void UInventoryComponent::Server_AddToTacticalsInventory_Implementation(ABasePickup * actor)
{
    //if(Role == ROLE_Authority)
    AddToTacticalsInventory(actor);
}

bool UInventoryComponent::Server_AddToTacticalsInventory_Validate(ABasePickup * actor)
{
    return true;
}

void UInventoryComponent::AddToTacticalsInventory(ABasePickup * actor)
{
    _inventory_tacticals.Add(actor);

}

void UInventoryComponent::Server_UpdateTacticalsInventory_Implementation()
{
    UpdateTacticalsInventory();
}

bool UInventoryComponent::Server_UpdateTacticalsInventory_Validate()
{
    return true;
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

    //GEngine->AddOnScreenDebugMessage(1, 3, FColor::White, *sInventory);
    //Cast<AMasterTrappersAlpha1Character>(GetOwner()->GetRootComponent()->GetAttachParent()->GetOwner())->OnUpdateInventory.Broadcast(_inventory_tacticals);
}

void UInventoryComponent::Server_RemoveFromTacticalInventory_Implementation()
{
    RemoveFromTacticalInventory();
}

bool UInventoryComponent::Server_RemoveFromTacticalInventory_Validate()
{
    return true;
}

void UInventoryComponent::RemoveFromTacticalInventory()
{
    //ABasePickup*& LastActor = _inventory_tacticals.Last();
    _inventory_tacticals.Last()->switchTexture();
    _inventory_tacticals.Last()->Name = "";
    _inventory_tacticals.Pop();

}


//Replicates UPROPERTIES
//void UInventoryComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
//{
//    Super::GetLifetimeReplicatedProps(OutLifetimeProps);
//
//    DOREPLIFETIME(UInventoryComponent, _inventory_tacticals);
//
//
//
//}