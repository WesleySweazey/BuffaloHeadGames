// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BaseAreaEffect.generated.h"

UCLASS()
class MASTERTRAPPERSALPHA1_API ABaseAreaEffect : public AActor
{
	GENERATED_BODY()
public:
    ABaseAreaEffect();
    virtual void PlayEffects();
    virtual void Stop();

    UPROPERTY(/*VisibleDefaultsOnly*/EditAnywhere, Category = Projectile)
        class USphereComponent* CollisionComp;
protected:
    // Sets default values for this actor's properties
    

    UParticleSystemComponent* ParticleZoneComponent;

    /** Sphere collision component */
    
    
    /** called when projectile hits something */
    UFUNCTION()
        void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);
    
    FTimerHandle LifeTimeHandle;


    // Called when the game starts or when spawned
    virtual void BeginPlay() override;

    // Called every frame
    virtual void Tick(float DeltaTime) override;

    /*UPROPERTY(VisibleAnywhere, Category = "MeshComponent",
        meta = (AllowPrivateAccess))
        class UStaticMeshComponent* StaticMeshComponent;*/
    UPROPERTY(EditAnywhere, Category = "FX")
        class UParticleSystem* Particles;
    UPROPERTY(EditAnywhere, Category = "FX")
        float LifeTime;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FX")
        class USoundCue* ExplosionSound;
    //UStaticMeshComponent* GetStaticMeshComponent();
};
