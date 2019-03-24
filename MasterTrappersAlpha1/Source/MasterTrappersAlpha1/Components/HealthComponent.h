// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "HealthComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class MASTERTRAPPERSALPHA1_API UHealthComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UHealthComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
    /** Float for maximum amount of health */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health")
        float FullHealth;
    /** Float for current amount of health */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health")
        float Health;
    /** Float for current health percent*/
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health")
        float HealthPercentage;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
    
    //take damage
    float TakeDamage(float DamageAmount);
    //reset health on death 
    void ResetHealth();
    /** Returns player's current health */
    UFUNCTION(BlueprintPure, Category = "Health")
        float GetHealth();
    /** Updates player's health */
    UFUNCTION(BlueprintCallable, Category = "Health")
        void UpdateHealth(float HealthChange);


    bool bCanBeDamaged;
		
	
};
