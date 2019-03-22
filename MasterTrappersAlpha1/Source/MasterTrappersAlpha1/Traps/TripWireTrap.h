// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseTrap.h"
#include "TripWireTrap.generated.h"

/**
 * 
 */
UCLASS()
class MASTERTRAPPERSALPHA1_API ATripWireTrap : public ABaseTrap
{
	GENERATED_BODY()
public:
    ATripWireTrap();

    UPROPERTY(EditAnywhere, Category = "FX")
        class UParticleSystem* ExplosionParticles;

    UPROPERTY(EditAnywhere, Category = "FX")
        class USoundCue* ExplosionSound;

    UPROPERTY(VisibleAnywhere, Category = "Collision",
        meta = (AllowPrivateAccess))
        class UBoxComponent* BoxComponent;

    UFUNCTION()
        void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent,
            AActor* OtherActor,
            UPrimitiveComponent* OtherComp,
            int32 OtherBodyIndex,
            bool bFromSweep,
            const FHitResult &SweepResult);
protected:
    // Called when the game starts or when spawned
    virtual void BeginPlay() override;
    

};
