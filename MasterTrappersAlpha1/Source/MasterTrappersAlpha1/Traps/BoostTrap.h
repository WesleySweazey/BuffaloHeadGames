// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseTrap.h"
#include "BoostTrap.generated.h"

/**
 * 
 */
UCLASS()
class MASTERTRAPPERSALPHA1_API ABoostTrap : public ABaseTrap
{
	GENERATED_BODY()
public:
    ABoostTrap();

    UPROPERTY(VisibleAnywhere, Category = "Collision",
        meta = (AllowPrivateAccess))
        class UBoxComponent* BoxComponent;

    UPROPERTY(EditAnywhere, Category = "Boost Amount")
        float boostSpeed = 1700.0f;

    UFUNCTION()
        void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent,
            AActor* OtherActor,
            UPrimitiveComponent* OtherComp,
            int32 OtherBodyIndex,
            bool bFromSweep,
            const FHitResult &SweepResult);
	
	
	
};
