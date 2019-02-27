// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseTrap.h"
#include "BananaPeelTrap.generated.h"

/**
 * 
 */
UCLASS()
class MASTERTRAPPERSALPHA1_API ABananaPeelTrap : public ABaseTrap
{
	GENERATED_BODY()
public:
    ABananaPeelTrap();

    UFUNCTION()
        void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent,
            AActor* OtherActor,
            UPrimitiveComponent* OtherComp,
            int32 OtherBodyIndex,
            bool bFromSweep,
            const FHitResult &SweepResult);

    UPROPERTY(VisibleAnywhere, Category = "Collision",
        meta = (AllowPrivateAccess))
        class USphereComponent* SphereComponent;
	
	
	
};
