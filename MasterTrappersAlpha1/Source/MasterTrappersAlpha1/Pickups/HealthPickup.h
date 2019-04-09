// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Pickups/BasePickup.h"
#include "HealthPickup.generated.h"

/**
 * 
 */
UCLASS()
class MASTERTRAPPERSALPHA1_API AHealthPickup : public ABasePickup
{
	GENERATED_BODY()
	
	
public:

    UFUNCTION()
        virtual void NotifyActorBeginOverlap(class AActor* OtherActor) override;

    virtual void OnInteract();
	
};
