// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Runtime/Engine/Classes/Particles/ParticleSystemComponent.h"
#include "BaseAreaEffect.generated.h"


UCLASS()
class MASTERTRAPPERSALPHA1_API ABaseAreaEffect : public AActor
{
	GENERATED_BODY()
public:
    ABaseAreaEffect();
    //Play Effects
    virtual void PlayEffects();
    UFUNCTION(NetMulticast, Reliable, WithValidation)
    virtual void Server_Stop();

    UPROPERTY(/*VisibleDefaultsOnly*/EditAnywhere, Category = Projectile)
        class USphereComponent* CollisionComp;
    //Set team
    void SetTeam(int currentTeam) { Team = currentTeam; }
protected:
    // Sets default values for this actor's properties
    UPROPERTY(EditAnywhere, Category = "FX", Replicated)
    UParticleSystemComponent* ParticleZoneComponent;

    /** called hits something */
    UFUNCTION()
        void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);
    
    //Timer handle for lifetime
    FTimerHandle LifeTimeHandle;
    //Timer handle for damage
    FTimerHandle damageTimeHandle;

    // Called when the game starts or when spawned
    virtual void BeginPlay() override;

    //Check Collision
    virtual void CheckCollision();

    // Called every frame
    virtual void Tick(float DeltaTime) override;
    //Particle System
    UPROPERTY(EditAnywhere, Category = "FX", Replicated)
        class UParticleSystem* Particles;
    //Lifetime till dies
    UPROPERTY(EditAnywhere, Category = "FX", Replicated)
        float LifeTime;
    //Sound effect
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FX", Replicated)
        class USoundCue* ExplosionSound;
    //Team
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Team", Replicated)
        int Team;
};
