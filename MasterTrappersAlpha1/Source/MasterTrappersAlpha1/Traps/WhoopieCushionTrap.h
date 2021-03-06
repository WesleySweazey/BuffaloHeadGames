// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseTrap.h"
#include "MasterTrappersAlpha1Character.h"
#include "WhoopieCushionTrap.generated.h"

/**
 * 
 */
UCLASS()
class MASTERTRAPPERSALPHA1_API AWhoopieCushionTrap : public ABaseTrap
{
	GENERATED_BODY()
public:
    AWhoopieCushionTrap();

    // Called when the game starts or when spawned
    virtual void BeginPlay() override;

    // Called every frame
    virtual void Tick(float DeltaTime) override;

    UPROPERTY(EditAnywhere, Category = "Explosion Detail")
        float Radius = 500.0f;

    UPROPERTY(EditAnywhere, Category = "Explosion Detail")
        float DetonationLength;

    /** Sphere collision component */
    UPROPERTY(/*VisibleDefaultsOnly*/EditAnywhere, Category = Projectile)
        class USphereComponent* CollisionComp;

    UPROPERTY(EditAnywhere, Category = "FX")
        class UParticleSystem* ExplosionParticles;

    UPROPERTY(EditAnywhere, Category = "FX")
        class USoundCue* ExplosionSound;

    /** Grenade Blueprint */
    UPROPERTY(EditDefaultsOnly,
        BlueprintReadOnly,
        Category = "AreaEffect")
        TSubclassOf<class AGasAreaEffect> GasAreaEffect;

    /** called when projectile hits something */
    UFUNCTION()
        void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent,
            AActor* OtherActor,
            UPrimitiveComponent* OtherComp,
            int32 OtherBodyIndex,
            bool bFromSweep,
            const FHitResult &SweepResult);

    UPROPERTY(VisibleAnywhere, Category = "Collision",
        meta = (AllowPrivateAccess))
        class USphereComponent* SphereComponent;

    /** Returns CollisionComp subobject **/
    FORCEINLINE class USphereComponent* GetCollisionComp() const { return CollisionComp; }

    void Detonate();
    bool bDetonated;

    UPROPERTY(VisibleAnywhere, Category = "Collision")
    TArray<AMasterTrappersAlpha1Character*> collidedCharacters;
};
