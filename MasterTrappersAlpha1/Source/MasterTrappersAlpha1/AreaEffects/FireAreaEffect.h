// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AreaEffects/BaseAreaEffect.h"
#include "FireAreaEffect.generated.h"

/**
 * 
 */
UCLASS()
class MASTERTRAPPERSALPHA1_API AFireAreaEffect : public ABaseAreaEffect
{
	GENERATED_BODY()
public:
    AFireAreaEffect();

protected:
    //Play effects
    virtual void PlayEffects() override;

    void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

    // Called when the game starts or when spawned
    virtual void BeginPlay() override;

    // Called every frame
    virtual void Tick(float DeltaTime) override;

    //Check Collision
    virtual void CheckCollision() override;
};
