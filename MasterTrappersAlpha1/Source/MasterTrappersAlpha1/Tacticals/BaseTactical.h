// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BaseTactical.generated.h"

UCLASS()
class MASTERTRAPPERSALPHA1_API ABaseTactical : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABaseTactical();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
    //Set Material
    virtual void SetMaterial(UMaterialInterface* mat) { TacticalMaterial = mat; }
    //Set Team
    void SetTeam(int value) { Team = value; }
    UPROPERTY(VisibleAnywhere, Category = "MeshComponent",
        meta = (AllowPrivateAccess))
        class UStaticMeshComponent* StaticMeshComponent;
    //Static mesh
    UStaticMeshComponent* GetStaticMeshComponent();
    //Material color
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Material, meta = (AllowPrivateAccess = "true"), Replicated)
        UMaterialInterface* TacticalMaterial;
    //Team
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Score", Replicated)
        int Team;
};
