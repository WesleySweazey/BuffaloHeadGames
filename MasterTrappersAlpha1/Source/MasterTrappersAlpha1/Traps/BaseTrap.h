// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BaseTrap.generated.h"

UCLASS()
class MASTERTRAPPERSALPHA1_API ABaseTrap : public AActor
{
    GENERATED_BODY()

public:
    // Sets default values for this actor's properties
    ABaseTrap();

    virtual void SetMaterial(UMaterialInterface* mat) { TrapMaterial = mat; }
    UFUNCTION(Server, Reliable, WithValidation)
        virtual void Server_SetMaterial(UMaterialInterface* mat);
    void SetTeam(int value) { Team = value; }
    /*UFUNCTION(NetMulticast, Reliable)
        virtual void Multicast_Destory();*/
protected:
    // Called when the game starts or when spawned
    virtual void BeginPlay() override;

    // Called every frame
    virtual void Tick(float DeltaTime) override;

    UPROPERTY(VisibleAnywhere, Category = "MeshComponent",
        meta = (AllowPrivateAccess))
        class UStaticMeshComponent* StaticMeshComponent;

    UStaticMeshComponent* GetStaticMeshComponent();
    //Material color
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Material, meta = (AllowPrivateAccess = "true"), Replicated)
        UMaterialInterface* TrapMaterial;
    //Team
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Score", Replicated)
        int Team;
};
