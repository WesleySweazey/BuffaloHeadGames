// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
//#include "Runtime//Engine/Classes/Sound/SoundCue.h"
#include "ThrowableLauncherProjectile.generated.h"

UCLASS(config=Game)
class AThrowableLauncherProjectile : public AActor
{
	GENERATED_BODY()

	/** Sphere collision component */
	UPROPERTY(/*VisibleDefaultsOnly*/EditAnywhere, Category=Projectile)
	class USphereComponent* CollisionComp;

	/** Projectile movement component */
	UPROPERTY(/*VisibleAnywhere, BlueprintReadOnly*/EditAnywhere, Category = Movement, meta = (AllowPrivateAccess = "true"))
	class UProjectileMovementComponent* ProjectileMovement;

    UPROPERTY(EditAnywhere, Category = "FX")
       class UParticleSystem* ExplosionParticles;

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
	AThrowableLauncherProjectile();

    virtual void BeginPlay() override;

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
};

