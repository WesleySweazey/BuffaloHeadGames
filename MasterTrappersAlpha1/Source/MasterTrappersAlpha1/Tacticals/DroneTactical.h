// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Tacticals/BaseTactical.h"
#include "DroneTactical.generated.h"

/**
 * 
 */
UCLASS()
class MASTERTRAPPERSALPHA1_API ADroneTactical : public ABaseTactical
{
	GENERATED_BODY()
	
	
public: 
    ADroneTactical();

    virtual void BeginPlay() override;

    //sound to be played when enemy is in radius
    UPROPERTY(EditAnywhere, Category = "FX")
        class USoundCue* TargetSpottedCue;

    UPROPERTY(EditAnywhere, Category = "FX")
        class UAudioComponent* DroneAudio;

    //enemy tracking radius
    UPROPERTY(VisibleAnywhere, Category = "Overlap")
        class USphereComponent* OverlapComponent;
    UFUNCTION()
        void StartOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult);
    UFUNCTION()
        void EndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
    // Called every frame
    virtual void Tick(float DeltaTime) override;
	
};
