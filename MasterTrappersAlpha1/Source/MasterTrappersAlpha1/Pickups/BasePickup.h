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
    //For float math
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

    //Rotation
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Pickup)
        FRotator RotationRate;
    //Mesh
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Pickup)
        class UStaticMeshComponent* ItemMesh;
    //BoxCollider
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Pickup)
        class UBoxComponent* BoxCollider;
    //Particles
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Pickup)
        class UParticleSystem* ExplosionParticles;
    //Sound
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Pickup)
        class USoundCue* ExplosionSound;
    //Name
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Pickup,Replicated)
        FString Name;
    //Icon image
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Pickup, Replicated)
        class UTexture2D* Image;
    //Blank image
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Pickup, Replicated)
        class UTexture2D* BlankImage;

    UPROPERTY(EditAnywhere, Category = Movement)
        float PitchValue;

    UPROPERTY(EditAnywhere, Category = Movement)
        float YawValue;

    UPROPERTY(EditAnywhere, Category = Movement)
        float RollValue;
    //Change icon image
    void switchTexture();
    //Rotates
    virtual void Rotator(float DeltaTime);

    //UFUNCTION()
    // void OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

   /* UFUNCTION()
        virtual void NotifyActorBeginOverlap(class AActor* OtherActor) override;*/
	
};
