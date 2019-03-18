// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BasePickup.generated.h"

UCLASS()
class MASTERTRAPPERSALPHA1_API ABasePickup : public AActor
{
	GENERATED_BODY()
	
private:
    float RunningTime;

public:
    // Sets default values for this actor's properties
    ABasePickup();

protected:
    // Called when the game starts or when spawned
    virtual void BeginPlay() override;

    void PlayEffects();

    virtual void Show(bool visible);

public:
    // Called every frame
    virtual void Tick(float DeltaTime) override;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Pickup)
        FRotator RotationRate;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Pickup)
        class UStaticMeshComponent* ItemMesh;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Pickup)
        class UBoxComponent* BoxCollider;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Pickup)
        class UParticleSystem* ExplosionParticles;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Pickup)
        class USoundCue* ExplosionSound;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Pickup)
        FString Name;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Pickup)
        class UTexture2D* Image;

    UPROPERTY(EditAnywhere, Category = Movement)
        float PitchValue;

    UPROPERTY(EditAnywhere, Category = Movement)
        float YawValue;

    UPROPERTY(EditAnywhere, Category = Movement)
        float RollValue;

    

    virtual void Rotator(float DeltaTime);

    //UFUNCTION()
    // void OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

   /* UFUNCTION()
        virtual void NotifyActorBeginOverlap(class AActor* OtherActor) override;*/
	
};
