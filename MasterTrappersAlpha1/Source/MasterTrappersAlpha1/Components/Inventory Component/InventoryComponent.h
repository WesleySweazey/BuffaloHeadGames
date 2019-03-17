// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "InventoryComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class MASTERTRAPPERSALPHA1_API UInventoryComponent : public UActorComponent
{
	GENERATED_BODY()
#pragma region Region_Private

#pragma endregion Region_Private

#pragma region Region_Public
public:
    // Sets default values for this component's properties
    UInventoryComponent();
    // Called every frame
    virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

    void AddToInventory(class APickupActor* pickup);
    class APickupActor* RemoveCurrentInventoryItem();

    int GetInventoryCount();

    /**
    * Getter for the currently equipped item.
    * @return Current item
    */
    UFUNCTION(BlueprintCallable, Category = "Inventory")
        class APickupActor* GetCurrentInventory() const;

    void ResetCurrentInventory();

#pragma endregion Region_Public

#pragma region Region_Protected
protected:
    // Called when the game starts
    virtual void BeginPlay() override;

    class AMasterTrappersAlpha1Character* CharacterOwner;

#pragma endregion Region_Protected


#pragma region Networking

public:
    UPROPERTY(Replicated)
        TArray< class APickupActor* > Inventory;
    /**
    * Cycle through the inventory array and equip the next item.
    */
    UFUNCTION(Reliable, Server, WithValidation, BlueprintCallable, Category = "Inventory")
        virtual void NextInventoryItem();

    /**
    * Cycle through the inventory array and equip the previous item.
    */
    UFUNCTION(Reliable, Server, WithValidation, BlueprintCallable, Category = "Inventory")
        virtual void PreviousInventoryItem();
protected:

    /** Item that is currently equipped */
    UPROPERTY(Replicated, VisibleAnywhere, BlueprintReadOnly, Category = "Inventory")
        class APickupActor* CurrentInventory;

#pragma endregion Networking Functions/Vars
	
};
