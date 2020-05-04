// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseTactical.h"
#include "MasterTrappersAlpha1Character.h"
#include "GrenadeTactical.generated.h"

/**
 *
 */
UCLASS()
class MASTERTRAPPERSALPHA1_API AGrenadeTactical : public ABaseTactical
{
    GENERATED_BODY()
public:
    //Explosion member
    UPROPERTY(VisibleAnywhere, Category = "Collision", Replicated)
        class UParticleSystemComponent* m_Explosion;

    /** Sphere collision component */
    UPROPERTY(/*VisibleDefaultsOnly*/EditAnywhere, Category = Projectile)
        class USphereComponent* CollisionComp;

    UPROPERTY(/*VisibleDefaultsOnly*/EditAnywhere, Category = Projectile)
        class USphereComponent* ExplosionComp;

    /** called when projectile hits something */
    UFUNCTION()
        void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent,
            AActor* OtherActor,
            UPrimitiveComponent* OtherComp,
            int32 OtherBodyIndex,
            bool bFromSweep,
            const FHitResult &SweepResult);

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
    AGrenadeTactical();
    //Timer handle
    FTimerHandle Explosionhandle;
    //On Explosion
    UFUNCTION(Server, Reliable, WithValidation)
        void Server_OnExplosion();
    //Colliding character
    UPROPERTY(VisibleAnywhere, Category = "Collision")
        TArray<AMasterTrappersAlpha1Character*> collidedCharacters;
    //Begin
    virtual void BeginPlay() override;
    //Detonation
    UFUNCTION()
        void OnDetonate();

    /** called when projectile hits something */
    UFUNCTION()
        void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

    /** Returns CollisionComp subobject **/
    FORCEINLINE class USphereComponent* GetCollisionComp() const { return CollisionComp; }
    /** Returns ProjectileMovement subobject **/
    FORCEINLINE class UProjectileMovementComponent* GetProjectileMovement() const { return ProjectileMovement; }

    // Called every frame
    virtual void Tick(float DeltaTime) override;

    //Timer handle before exploding
    FTimerHandle ExplodeHandle;

    //Lifetime
    float Lifetime;
};
