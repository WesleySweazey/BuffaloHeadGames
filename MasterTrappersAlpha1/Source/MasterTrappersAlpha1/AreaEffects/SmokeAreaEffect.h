// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AreaEffects/BaseAreaEffect.h"
#include "SmokeAreaEffect.generated.h"

/**
 * 
 */
UCLASS()
class MASTERTRAPPERSALPHA1_API ASmokeAreaEffect : public ABaseAreaEffect
{
	GENERATED_BODY()
public:
    ASmokeAreaEffect();

protected:
    //Play effects
    virtual void PlayEffects() override;

    // Called when the game starts or when spawned
    virtual void BeginPlay() override;

    // Called every frame
    virtual void Tick(float DeltaTime) override;

};
