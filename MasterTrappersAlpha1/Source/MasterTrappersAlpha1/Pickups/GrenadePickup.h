// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Pickups/BasePickup.h"
#include "GrenadePickup.generated.h"

/**
 * 
 */
UCLASS()
class MASTERTRAPPERSALPHA1_API AGrenadePickup : public ABasePickup
{
	GENERATED_BODY()
	
	
    UFUNCTION()
        virtual void NotifyActorBeginOverlap(class AActor* OtherActor) override;
	
    virtual void OnInteract();

};
