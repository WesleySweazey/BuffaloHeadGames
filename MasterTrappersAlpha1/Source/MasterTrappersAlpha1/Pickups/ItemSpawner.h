// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ItemSpawner.generated.h"

UCLASS()
class MASTERTRAPPERSALPHA1_API AItemSpawner : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AItemSpawner();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
    /** Random Pickup Blueprint */
    UPROPERTY(EditDefaultsOnly)
    USceneComponent* SceneComponent;

    UPROPERTY(VisibleAnywhere, Category = "Collision",
        meta = (AllowPrivateAccess))
        class USphereComponent* SphereComponent;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

    //Timer handle for next item spawn time
    FTimerHandle SpawnerWait;

    void SpawnItem();

    /** Random Pickup Blueprint */
    UPROPERTY(EditDefaultsOnly,
        BlueprintReadOnly,
        Category = "RandomPickup")
        TSubclassOf<class ARandomPickup> RandomPickup;

    UFUNCTION()
        void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent,
            AActor* OtherActor,
            UPrimitiveComponent* OtherComp,
            int32 OtherBodyIndex,
            bool bFromSweep,
            const FHitResult &SweepResult);
};
