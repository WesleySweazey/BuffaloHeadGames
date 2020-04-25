// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseTactical.h"
#include "FlashBangTactical.generated.h"

/**
 * 
 */
UCLASS()
class MASTERTRAPPERSALPHA1_API AFlashBangTactical : public ABaseTactical
{
	GENERATED_BODY()
public:
    ~AFlashBangTactical();

    /** Sphere collision component */
    UPROPERTY(/*VisibleDefaultsOnly*/EditAnywhere, Category = Projectile)
        class USphereComponent* CollisionComp;

    /** Explosion radius component*/
    UPROPERTY(EditAnywhere, Category = Projectile)
        class USphereComponent* ExplosionComp;

    /** Projectile movement component */
    UPROPERTY(/*VisibleAnywhere, BlueprintReadOnly*/EditAnywhere, Category = Movement, meta = (AllowPrivateAccess = "true"))
        class UProjectileMovementComponent* ProjectileMovement;
    
    //Particle
    UPROPERTY(EditAnywhere, Category = "FX")
        class UParticleSystem* SmokeParticles;
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

    //Smoke begin
    FTimerHandle smokeTimerHandle;
    //Spawn smoke
    void StartSmoke();

    /** Smoke effect Blueprint */
    UPROPERTY(EditDefaultsOnly,
        BlueprintReadOnly,
        Category = "AreaEffect")
        TSubclassOf<class ASmokeAreaEffect> SmokeEffect;
public:
    AFlashBangTactical();
    //Begin
    virtual void BeginPlay() override;
    //Detonation
    UFUNCTION(Server, Reliable, WithValidation)
        void Server_OnDetonate();

    /** called when projectile hits something */
    UFUNCTION()
        void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

    /** Returns CollisionComp subobject **/
    FORCEINLINE class USphereComponent* GetCollisionComp() const { return CollisionComp; }
    /** Returns ProjectileMovement subobject **/
    FORCEINLINE class UProjectileMovementComponent* GetProjectileMovement() const { return ProjectileMovement; }

    // Called every frame
    virtual void Tick(float DeltaTime) override;
};
