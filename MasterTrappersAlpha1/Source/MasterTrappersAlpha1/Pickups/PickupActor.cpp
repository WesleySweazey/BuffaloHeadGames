// Fill out your copyright notice in the Description page of Project Settings.

#include "PickupActor.h"
#include <Components/SkeletalMeshComponent.h>
#include <Components/PrimitiveComponent.h>
#include "Net/UnrealNetwork.h"
#include "Blueprint/UserWidget.h"

// Sets default values
APickupActor::APickupActor()
{
    // Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = true;
    //NET CODE 1 NET PICKUP
    SetReplicates(true);
    SetReplicateMovement(true);

}

// Called when the game starts or when spawned
void APickupActor::BeginPlay()
{
    Super::BeginPlay();

}

void APickupActor::PostInitializeComponents()
{
    Super::PostInitializeComponents();

    // Get the root primitive component.
    Primitive = Cast<UPrimitiveComponent>(GetRootComponent());
    if (Primitive)
    {
        if (PickupMode == EPickupMode::AutoPickup)
        {
            Primitive->SetCollisionEnabled(ECollisionEnabled::NoCollision);
            Primitive->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
            Primitive->SetCollisionResponseToAllChannels(ECR_Ignore);
            Primitive->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap); //??? Or whateever other Object
        }
    }

    if (WBP_PickupIcon)
    {
        PickupIcon = CreateWidget<UUserWidget>(GetWorld(), WBP_PickupIcon);
        //PickupIcon->GetWidgetFromName("Button");
    }
    //check(Primitive != nullptr && "Actor is mising a primitive component!");
}

// Called every frame
void APickupActor::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}

void APickupActor::NotifyActorBeginOverlap(AActor* OtherActor)
{
    Super::NotifyActorBeginOverlap(OtherActor);
}

void APickupActor::DisplayInventoryIcon_Implementation(bool visible)
{
    if (PickupIcon->IsValidLowLevel())
    {
        if (visible)
            PickupIcon->AddToViewport();
        else
            PickupIcon->RemoveFromViewport();
    }
    else
    {
        if (WBP_PickupIcon)
        {
            PickupIcon = CreateWidget<UUserWidget>(GetWorld(), WBP_PickupIcon);
            if (visible)
                PickupIcon->AddToViewport();
            //PickupIcon->GetWidgetFromName("Button");
        }

    }
}

void APickupActor::Disable_Implementation()
{
    if (Primitive)
    {
        this->SetActorHiddenInGame(true);
        Primitive->SetActive(false);
        //Primitive->SetSimulatePhysics(false);
        Primitive->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    }
}

//bool APickupActor::Disable_Validate()
//{
//	return true;
//}

void APickupActor::Enable_Implementation()
{
    if (Primitive)
    {
        this->SetActorHiddenInGame(false);
        Primitive->SetActive(true);
        //Primitive->SetSimulatePhysics(true);
        Primitive->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
    }
}

//bool APickupActor::Enable_Validate()
//{
//	return true;
//}

void APickupActor::PickupIconClicked()
{

}

void APickupActor::ShowSelect(bool selected)
{
    if (Primitive)
        Primitive->SetRenderCustomDepth(selected);
}
