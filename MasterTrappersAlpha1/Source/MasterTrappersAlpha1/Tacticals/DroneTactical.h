// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Tacticals/BaseTactical.h"
#include "MasterTrappersAlpha1Character.h"
#include "DroneTactical.generated.h"

/**
 * 
 */
UCLASS()
class MASTERTRAPPERSALPHA1_API ADroneTactical : public ABaseTactical
{
	GENERATED_BODY()
	
private:
    //For float math
    float RunningTime;
public: 
    //Constructor
    ADroneTactical();
    //Begin
    virtual void BeginPlay() override;

    //sound to be played when enemy is in radius
    UPROPERTY(EditAnywhere, Category = "FX")
        class USoundCue* TargetSpottedCue;

    UPROPERTY(EditAnywhere, Category = "FX")
        class UAudioComponent* DroneAudio;

    //enemy tracking radius
    UPROPERTY(VisibleAnywhere, Category = "Overlap")
        class USphereComponent* OverlapComponent;
    //raycasting variables for drone to detect if wall is in front of it to turn in a random direction
    
    //Facing direction
    FVector DroneDir;
    
    //Smoke begin
    FTimerHandle droneTimerHandle;

    AMasterTrappersAlpha1Character* DroneOwner;

    //Character overlapping - play sound
    UFUNCTION()
        void StartOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult);
    UFUNCTION()
        void EndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
    //Dumb AI
    UFUNCTION()
        bool CheckForWalls(AMasterTrappersAlpha1Character* pawn);
    // Called every frame
    virtual void Tick(float DeltaTime) override;
	
    void SelfDestruct();
};
