// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "InventoryComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class MASTERTRAPPERSALPHA1_API UInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

        //Inventory
private:
    /** Inventory of Pick up Actors */
    TArray<class ABasePickup*> _inventory_tacticals;

public:	
	// Sets default values for this component's properties
	UInventoryComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

    /** Add item to inventory*/
    void AddToTacticalsInventory(class ABasePickup* actor);
	

    /** Update inventory*/
    UFUNCTION(BlueprintCallable)
        void UpdateTacticalsInventory();

    // remove last element from inventory
    void RemoveFromTacticalInventory();
};
