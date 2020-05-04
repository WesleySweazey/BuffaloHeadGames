// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseTactical.h"
#include "MolotovTactical.generated.h"

/**
 * 
 */
UCLASS()
class MASTERTRAPPERSALPHA1_API AMolotovTactical : public ABaseTactical
{
	GENERATED_BODY()
public:
    /** Sphere collision component */
    UPROPERTY(/*VisibleDefaultsOnly*/EditAnywhere, Category = Projectile)
        class USphereComponent* CollisionComp;

    /** Projectile movement component */
    UPROPERTY(/*VisibleAnywhere, BlueprintReadOnly*/EditAnywhere, Category = Movement, meta = (AllowPrivateAccess = "true"))
        class UProjectileMovementComponent* ProjectileMovement;
    //Particle
    UPROPERTY(EditAnywhere, Category = "FX")
        class UParticleSystem* ExplosionParticles;
    //Sound
    UPROPERTY(EditAnywhere, Category = "FX")
        class USoundCue* ExplosionSound;

    UPROPERTY(EditAnywhere, Category = "Projectile")
        float Radius = 500.0f;

    UPROPERTY(EditAnywhere, Category = "pitch")
        float PitchVal;

    UPROPERTY(EditAnywhere, Category = "yaw")
        float YawVal;

    UPROPERTY(EditAnywhere, Category = "roll")
        float RollVal;

public:
    UPROPERTY(EditAnywhere, Category = "Projectile")
        FVector FowardVelocity;

    AMolotovTactical();
    //Begin
    virtual void BeginPlay() override;

    //On Detonate spawns fire
    UFUNCTION()
        void OnDetonate();
    
    /** Fire Area effect Blueprint */
    UPROPERTY(EditDefaultsOnly,
        BlueprintReadOnly,
        Category = "AreaEffect")
        TSubclassOf<class AFireAreaEffect> FireAreaEffect;


    //On Explosion
    UFUNCTION(Server, Reliable, WithValidation)
        void Server_OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);
    ///** called when projectile hits something */
    //UFUNCTION()
    //    void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

    /** Returns CollisionComp subobject **/
    FORCEINLINE class USphereComponent* GetCollisionComp() const { return CollisionComp; }
    /** Returns ProjectileMovement subobject **/
    FORCEINLINE class UProjectileMovementComponent* GetProjectileMovement() const { return ProjectileMovement; }

    // Called every frame
    virtual void Tick(float DeltaTime) override;
};
