// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AreaEffects/BaseAreaEffect.h"
#include "GasAreaEffect.generated.h"

/**
 * 
 */
UCLASS()
class MASTERTRAPPERSALPHA1_API AGasAreaEffect : public ABaseAreaEffect
{
	GENERATED_BODY()
public:
    AGasAreaEffect();
protected:
    //Play effects
    virtual void PlayEffects() override;
    //On hit
        void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

        // Called when the game starts or when spawned
        virtual void BeginPlay() override;

        // Called every frame
        virtual void Tick(float DeltaTime) override;

        //Check Collision
        UFUNCTION(Server, Reliable, WithValidation)
            void Server_CheckCollision();
};
