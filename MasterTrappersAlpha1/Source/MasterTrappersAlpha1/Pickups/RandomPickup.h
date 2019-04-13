// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Pickups/BasePickup.h"
#include "RandomPickup.generated.h"

/**
 * 
 */
UCLASS()
class MASTERTRAPPERSALPHA1_API ARandomPickup : public ABasePickup
{
	GENERATED_BODY()
	
public:
    //Over lapping
        UFUNCTION()
        virtual void NotifyActorBeginOverlap(class AActor* OtherActor) override;

    //UFUNCTION(BlueprintCallable, Server, Reliable, WithValidation)
    //    void Server_OnInteract();
    //Interact Function
    virtual void OnInteract(int pickupIdx);
	
};
