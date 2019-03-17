// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PickupActor.generated.h"

USTRUCT(BlueprintType)
struct FPickupInventoryConfig
{
    GENERATED_USTRUCT_BODY();

    /** Class of the item to create */
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = GamePlay1)
        TSubclassOf<class APickupActor> PickupActorClass;

    /** Whether or not this item will be equipped on start */
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = AdvKit)
        bool bEquip;
};

UENUM(BlueprintType)
enum class EPickupMode : uint8
{
    RayCast,
    AutoPickup,
    PickupEquip//,
               //Default
};

UCLASS()
class MASTERTRAPPERSALPHA1_API APickupActor : public AActor
{
    GENERATED_BODY()
#pragma region Region_Private

#pragma endregion Region_Private

#pragma region Region_Public
public:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Pickup Mode")
        EPickupMode PickupMode = EPickupMode::RayCast;


    // Sets default values for this actor's properties
    APickupActor();
    void ShowSelect(bool selected);

    virtual void Tick(float DeltaTime) override;
    virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;


    UFUNCTION(Client, Reliable)
        virtual void DisplayInventoryIcon(bool visible);

    UFUNCTION(NetMulticast, Reliable)//, WithValidation)
        virtual void Disable();

    UFUNCTION(NetMulticast, Reliable)//, WithValidation)
        virtual void Enable();

    UFUNCTION()
        virtual void PickupIconClicked();
#pragma endregion Region_Public

#pragma region Region_Protected
protected:
    class UPrimitiveComponent* Primitive;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widgets")
        class TSubclassOf<UUserWidget> WBP_PickupIcon;

    UPROPERTY()
        class UUserWidget* PickupIcon;

    // Called when the game starts or when spawned
    virtual void BeginPlay() override;
    virtual void PostInitializeComponents() override;
#pragma endregion Region_Protected
};
