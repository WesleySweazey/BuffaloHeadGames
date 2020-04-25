// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#include "MasterTrappersAlpha1Character.h"
#include "MasterTrappersAlpha1Projectile.h"
#include "UObject/ConstructorHelpers.h"
#include "Animation/AnimInstance.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "Components/DecalComponent.h"
#include "GameFramework/InputSettings.h"
#include "HeadMountedDisplayFunctionLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "MotionControllerComponent.h"
#include "XRMotionControllerBase.h" // for FXRMotionControllerBase::RightHandSourceId
#include "Engine/World.h"
//#include "Runtime/Engine/Public/TimerManager.h"
#include "Components/PostProcessComponent.h"
#include "Materials/Material.h"
#include "Traps/BaseTrap.h"
#include "Traps/BearTrap.h"
#include "Traps/C4Trap.h"
#include "Traps/BoostTrap.h"
#include "Traps/TripWireTrap.h"
#include "Traps/BananaPeelTrap.h"
#include "Traps/WhoopieCushionTrap.h"
#include "Tacticals/GrenadeTactical.h"
#include "Tacticals/FlashBangTactical.h"
#include "Tacticals/MolotovTactical.h"
#include "Tacticals/NinjaStarTactical.h"
#include "Tacticals/ThrowingAxeTactical.h"
#include "DrawDebugHelpers.h"
#include <string>
#include "Pickups/BasePickup.h"
#include "Components/InventoryComponent.h"
#include "Components/HealthComponent.h"
#include "Net/UnrealNetwork.h"
#include "MasterTrappersAlpha1/MasterTrappersGameStateBase.h"
#include "MasterTrappersAlpha1/MasterTrappersAlpha1GameMode.h"
#include "Runtime/Core/Public/Math/UnrealMathUtility.h"
#include "Runtime/Engine/Classes/GameFramework/Actor.h"
#include "Runtime/Engine/Classes/Materials/MaterialInstanceDynamic.h"
#include "Tacticals/DroneTactical.h"
#include "Runtime/Engine/Public/TimerManager.h"
#include "GameFrwkSessionsPlugin/LobbyGameMode.h"

DEFINE_LOG_CATEGORY_STATIC(LogFPChar, Warning, All);

//////////////////////////////////////////////////////////////////////////
// AMasterTrappersAlpha1Character

AMasterTrappersAlpha1Character::AMasterTrappersAlpha1Character()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(55.f, 96.0f);

	// set our turn rates for input
	BaseTurnRate = 45.f;
	BaseLookUpRate = 45.f;

	// Create a CameraComponent	
	FirstPersonCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
	FirstPersonCameraComponent->SetupAttachment(GetCapsuleComponent());
	FirstPersonCameraComponent->RelativeLocation = FVector(-39.56f, 1.75f, 64.f); // Position the camera
	FirstPersonCameraComponent->bUsePawnControlRotation = true;

    FP_PostProcessComponent = CreateDefaultSubobject<UPostProcessComponent>(TEXT("PostProcessComponent"));
    //FP_PostProcessComponent->SetIsReplicated(true);

	// Create a mesh component that will be used when being viewed from a '1st person' view (when controlling this pawn)
	Mesh1P = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("CharacterMesh1P"));
	//Mesh1P->SetOnlyOwnerSee(true);
	Mesh1P->SetupAttachment(FirstPersonCameraComponent);
	Mesh1P->bCastDynamicShadow = false;
	Mesh1P->CastShadow = false;
	Mesh1P->RelativeRotation = FRotator(1.9f, -19.19f, 5.2f);
	Mesh1P->RelativeLocation = FVector(-0.5f, -4.4f, -155.7f);

	// Create a gun mesh component
	FP_Gun = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("FP_Gun"));
	FP_Gun->SetOnlyOwnerSee(true);			// only the owning player will see this mesh
	FP_Gun->bCastDynamicShadow = false;
	FP_Gun->CastShadow = false;
	// FP_Gun->SetupAttachment(Mesh1P, TEXT("GripPoint"));
	FP_Gun->SetupAttachment(RootComponent);

	FP_MuzzleLocation = CreateDefaultSubobject<USceneComponent>(TEXT("MuzzleLocation"));
	FP_MuzzleLocation->SetupAttachment(FP_Gun);
	FP_MuzzleLocation->SetRelativeLocation(FVector(0.2f, 48.4f, -10.6f));

	// Default offset from the character location for projectiles to spawn
	GunOffset = FVector(100.0f, 0.0f, 10.0f);

    // Create a decal in the world to show the cursor's location
    CursorToWorld = CreateDefaultSubobject<UDecalComponent>("CursorToWorld");
    CursorToWorld->SetupAttachment(RootComponent);
    CursorToWorld->DecalSize = FVector(16.0f, 32.0f, 32.0f);
    CursorToWorld->SetRelativeRotation(FRotator(90.0f, 0.0f, 0.0f).Quaternion());
    CursorToWorld->SetIsReplicated(true);

    //Intializing UProperties
    ShoveStrength = 15000.0f;
    Speed = 3.0f;
    totalTacticals = 5;
    currentTactical = 0;
    totalTraps = 5;
    currentTrap = 0;
    
    Team = 0;

    CurrentSpeed = 0.0f;

    //Adding Player Tag
    Tags.Add("Player");

    //create inventory component
    InventoryComponent = CreateDefaultSubobject<UInventoryComponent>("Inventory Component");
    

    //create health component
    HealthComponent = CreateDefaultSubobject<UHealthComponent>("Health Component");
    
    //Initialize grenade number
    GrenadeNum = 0;
    BearTrapNum = 0;
    DroneTacticalNum = 0;
    FlashBangNum = 10; 
    MolotovNum = 0;
    NinjaStarNum = 0;
    ThrowingAxeNum = 10;
    BananaPeelNum = 0;
    C4TrapNum = 0;
    TripWireTrapNum = 0;
    WhoopieCushionTrapNum = 0;
    BoostTrapNum = 0;

    //initialize health pickup
    MaxHealthPickupNum = 10;
    CurrentHealthPickupNum = 0;

    SetReplicates(true);
    SetReplicateMovement(true);


    //Add all possible spawn location
    TArray<AActor*> playerStarts;
    UGameplayStatics::GetAllActorsOfClass(GetWorld(), APlayerStart::StaticClass(), playerStarts);
    //NewPlayer->StartSpot = playerStarts[Team];
    for (int i = 0; i < playerStarts.Num(); i++)
    {
        RespawnLocations.Add(playerStarts[i]->GetActorLocation());
    }

    SavedPosition = FirstPersonCameraComponent->GetComponentLocation();
}
// Server Add Inventory Functions
void AMasterTrappersAlpha1Character::Server_AddToInventory_Implementation(ABasePickup * actor)
{
    AddToInventory(actor);
}

bool AMasterTrappersAlpha1Character::Server_AddToInventory_Validate(ABasePickup * actor)
{
    return true;
}


void AMasterTrappersAlpha1Character::AddToInventory(ABasePickup * actor)
{
    if(Role==ROLE_Authority)
    InventoryComponent->Server_AddToTacticalsInventory(actor);
    
}

//void AMasterTrappersAlpha1Character::RemoveFromInventory(ABasePickup * actor)
//{
//    _inventory.Add(actor);
//
//}

// Server Update Inventory Functions

void AMasterTrappersAlpha1Character::Server_UpdateInventory_Implementation()
{
    UpdateInventory();
}

void AMasterTrappersAlpha1Character::UpdateInventory()
{
    if (Role == ROLE_Authority)
    {
        InventoryComponent->Server_UpdateTacticalsInventory();
        OnUpdateInventory.Broadcast(InventoryComponent->_inventory_tacticals);
    }
}

bool AMasterTrappersAlpha1Character::Server_UpdateInventory_Validate()
{
    return true;
}

// Server Switch Tactical Functions
bool AMasterTrappersAlpha1Character::Server_SwitchTacticalUp_Validate()
{
    return true;
}

void AMasterTrappersAlpha1Character::Server_SwitchTacticalUp_Implementation()
{
    //Switch the inventory of Tacticals to the next index 
    currentTactical++;
    if (currentTactical > totalTacticals)
    {
        currentTactical = 0;
    }
}

bool AMasterTrappersAlpha1Character::Server_SwitchTacticalDown_Validate()
{
    return true;
}


void AMasterTrappersAlpha1Character::Server_SwitchTacticalDown_Implementation()
{
    //Switch the inventory of traps to the next index 
    currentTactical--;
    if (currentTactical < 0)
    {
        currentTactical = totalTacticals;
    }
}

// Spawn Trap
bool AMasterTrappersAlpha1Character::SpawnTrap_Validate()
{
    return true;
}

void AMasterTrappersAlpha1Character::SpawnTrap_Implementation()
{
    if (Role == ROLE_Authority)
    {
    // Switch statement uses the current trap index
    switch (currentTrap)
    {
    case 0:
        if (BearTrap != nullptr && BearTrapNum>0)
        {
            UWorld* const World = GetWorld();
            if (World)
            {
                //Creates Spawn Parameters, Transforms and Rotations
                FHitResult HitResult;
                FCollisionQueryParams Params(NAME_None, FCollisionQueryParams::GetUnknownStatId());
                FActorSpawnParameters SpawnParams;
                SpawnParams.Owner = this;
                SpawnParams.SpawnCollisionHandlingOverride =
                    ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
                FTransform SpawnTransform = CursorToWorld->GetComponentTransform();

                ABearTrap* SpawnedActor = World->SpawnActor<ABearTrap>(BearTrap, SpawnTransform, SpawnParams);
                if (SpawnedActor)
                {
                //Set Rotation
                SpawnedActor->SetActorRelativeRotation(GetTrapSpawnRotation());
                //Set Color & Team
                SpawnedActor->SetMaterial(CharacterMaterial);
                SpawnedActor->Server_SetMaterial(CharacterMaterial);
                SpawnedActor->SetTeam(Team);
                BearTrapNum--;

                    UE_LOG(LogTemp, Warning, TEXT("Bear Trap Spawned"));
                }
            }
        }
        break;
    case 1:
        if (BoostTrap != nullptr && BoostTrapNum>0)
        {
            UWorld* const World = GetWorld();
            if (World)
            {
                //Creates Spawn Parameters, Transforms and Rotations
                FHitResult HitResult;
                FCollisionQueryParams Params(NAME_None, FCollisionQueryParams::GetUnknownStatId());
                FActorSpawnParameters SpawnParams;
                SpawnParams.Owner = this;
                SpawnParams.SpawnCollisionHandlingOverride =
                    ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
                FTransform SpawnTransform = CursorToWorld->GetComponentTransform();

                ABoostTrap* SpawnedActor = World->SpawnActor<ABoostTrap>(BoostTrap, SpawnTransform, SpawnParams);
                if (SpawnedActor)
                {
                //Set Rotation
                SpawnedActor->SetActorRelativeRotation(GetTrapSpawnRotation());
                //Set Color & Team
                SpawnedActor->SetMaterial(CharacterMaterial);
                SpawnedActor->Server_SetMaterial(CharacterMaterial);
                SpawnedActor->SetTeam(Team);
                BoostTrapNum--;
                
                    UE_LOG(LogTemp, Warning, TEXT("Boost Pad Trap Spawned"));
                }
            }
        }
        break;
    case 2:
        if (C4Trap != nullptr && C4TrapNum>0)
        {
            UWorld* const World = GetWorld();
            if (World)
            {
                //Creates Spawn Parameters, Transforms and Rotations
                FHitResult HitResult;
                FCollisionQueryParams Params(NAME_None, FCollisionQueryParams::GetUnknownStatId());
                FActorSpawnParameters SpawnParams;
                SpawnParams.Owner = this;
                SpawnParams.SpawnCollisionHandlingOverride =
                    ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
                FTransform SpawnTransform = CursorToWorld->GetComponentTransform();

                AC4Trap* SpawnedActor = World->SpawnActor<AC4Trap>(C4Trap, SpawnTransform, SpawnParams);
                if (SpawnedActor)
                {
                //Set Rotation
                SpawnedActor->SetActorRelativeRotation(GetTrapSpawnRotation());
                //Set Color & Team
                SpawnedActor->SetMaterial(CharacterMaterial);
                SpawnedActor->Server_SetMaterial(CharacterMaterial);
                SpawnedActor->SetTeam(Team);
                C4TrapNum--;
                // Add C4 Trap to TArray of C4 traps already placed
                PlacedC4Traps.Add(SpawnedActor);
                
                    UE_LOG(LogTemp, Warning, TEXT("C4 Trap Spawned"));
                }
            }
        }
        break;
    case 3:
        if (TripWireTrap != nullptr && TripWireTrapNum>0)
        {
            UWorld* const World = GetWorld();
            if (World)
            {
                //Creates Spawn Parameters, Transforms and Rotations
                FHitResult HitResult;
                FCollisionQueryParams Params(NAME_None, FCollisionQueryParams::GetUnknownStatId());
                FActorSpawnParameters SpawnParams;
                SpawnParams.Owner = this;
                SpawnParams.SpawnCollisionHandlingOverride =
                    ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
                FTransform SpawnTransform = CursorToWorld->GetComponentTransform();

                ATripWireTrap* SpawnedActor = World->SpawnActor<ATripWireTrap>(TripWireTrap, SpawnTransform, SpawnParams);
                if (SpawnedActor)
                {
                //Set Rotation
                SpawnedActor->SetActorRelativeRotation(GetTrapSpawnRotation());
                //Set Color & Team
                SpawnedActor->SetMaterial(CharacterMaterial);
                SpawnedActor->Server_SetMaterial(CharacterMaterial);
                SpawnedActor->SetTeam(Team);
                TripWireTrapNum--;
                
                    UE_LOG(LogTemp, Warning, TEXT("Trip Wire Trap Spawned"));
                }
            }
        }
        break;
    case 4:
        if (BananaPeelTrap != nullptr && BananaPeelNum>0)
        {
            UWorld* const World = GetWorld();
            if (World)
            {
                FHitResult HitResult;
                FCollisionQueryParams Params(NAME_None, FCollisionQueryParams::GetUnknownStatId());
                FActorSpawnParameters SpawnParams;
                SpawnParams.Owner = this;
                SpawnParams.SpawnCollisionHandlingOverride =
                    ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
                FTransform SpawnTransform = CursorToWorld->GetComponentTransform();

                ABananaPeelTrap* SpawnedActor = World->SpawnActor<ABananaPeelTrap>(BananaPeelTrap, SpawnTransform, SpawnParams);
                if (SpawnedActor)
                {
                //Set Rotation
                SpawnedActor->SetActorRelativeRotation(GetTrapSpawnRotation());
                //Set Color & Team
                SpawnedActor->SetMaterial(CharacterMaterial);
                SpawnedActor->Server_SetMaterial(CharacterMaterial);
                SpawnedActor->SetTeam(Team);
                BananaPeelNum--;
                
                    UE_LOG(LogTemp, Warning, TEXT("Banana Peel Trap Spawned"));
                }
            }
        }
        break;
    case 5:
        if (WhoopieCushionTrap != nullptr && WhoopieCushionTrapNum>0)
        {
            UWorld* const World = GetWorld();
            if (World)
            {
                FHitResult HitResult;
                FCollisionQueryParams Params(NAME_None, FCollisionQueryParams::GetUnknownStatId());
                FActorSpawnParameters SpawnParams;
                SpawnParams.Owner = this;
                SpawnParams.SpawnCollisionHandlingOverride =
                    ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
                FTransform SpawnTransform = CursorToWorld->GetComponentTransform();

                AWhoopieCushionTrap* SpawnedActor = World->SpawnActor<AWhoopieCushionTrap>(WhoopieCushionTrap, SpawnTransform, SpawnParams);
                if(SpawnedActor)
                {
                //Set Rotation
                SpawnedActor->SetActorRelativeRotation(GetTrapSpawnRotation());
                //Set Color & Team
                SpawnedActor->SetMaterial(CharacterMaterial);
                SpawnedActor->Server_SetMaterial(CharacterMaterial);
                SpawnedActor->SetTeam(Team);
                WhoopieCushionTrapNum--;
                    UE_LOG(LogTemp, Warning, TEXT("Whoopie Cushion Trap Spawned"));
                }
            }
        }
        break;
    }
}
}

//Activate traps

bool AMasterTrappersAlpha1Character::ActivateTrap_Validate()
{
    return true;
}

void AMasterTrappersAlpha1Character::ActivateTrap_Implementation()
{
    if (Role == ROLE_Authority)
    {
        // If a C4 trap has been placed it will be a detonated
        if (PlacedC4Traps.Num() > 0)
        {
            for (int i = 0; i < PlacedC4Traps.Num(); i++)
            {
                UE_LOG(LogTemp, Warning, TEXT("C4 Activated"));
                PlacedC4Traps[i]->Detonate();
                PlacedC4Traps.RemoveAt(i);
            }
        }
    }
}

//Server switch traps

bool AMasterTrappersAlpha1Character::Server_SwitchTrapUp_Validate()
{
    return true;
}

void AMasterTrappersAlpha1Character::Server_SwitchTrapUp_Implementation()
{
    //Switch the inventory of traps to the next index 
    currentTrap++;
    if (currentTrap > totalTraps)
    {
        currentTrap = 0;
    }
}

bool AMasterTrappersAlpha1Character::Server_SwitchTrapDown_Validate()
{
    return true;
}

void AMasterTrappersAlpha1Character::Server_SwitchTrapDown_Implementation()
{
    //Switch the inventory of traps to the next index 
    currentTrap--;
    if (currentTrap < 0)
    {
        currentTrap = totalTraps;
    }
}

void AMasterTrappersAlpha1Character::PostBeginPlay()
{
    //make a material Instance of this master material. Do NOT do this in the constructor, only after post init.
    UMaterialInstanceDynamic* RV_MatInst = UMaterialInstanceDynamic::Create(MasterMaterialRef, this);
    /*if (GetWorld()->GetNetMode() != NM_DedicatedServer)
    {*/
        //if (Team == 0)
        //{
            //Team = 1;
    if (Role == ROLE_Authority)
    {
        this->AssignTeams();
        //Server_SetColor();
        this->Multicast_AssignColors();
    }
    else
    {
        Mesh1P->SetMaterial(0, CharacterMaterial);
    }
        //}
    //}
    //After you make your custom T2D, assign it as a texture parameter to your material
    //RV_MatInst->SetTextureParameterValue(FName("T2DParam"), YourCustomT2DRef);
    //ref copy: Texture2D'/Game/Inventory/hp.hp'
}

//Server Slip Functions

bool AMasterTrappersAlpha1Character::Server_StartSlip_Validate()
{
    return true;
}

void AMasterTrappersAlpha1Character::Server_StartSlip_Implementation()
{
    UWorld* const World = GetWorld();
    World->GetTimerManager().SetTimer(BananaSlipTimerHandle, this, &AMasterTrappersAlpha1Character::Server_EndSlip, BananaSlipTimerLength, false);
    APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
    ACharacter* character = Cast<ACharacter>(this);
    character->DisableInput(PlayerController);
    //GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, "Start Slip");
}

bool AMasterTrappersAlpha1Character::Server_EndSlip_Validate()
{
    return true;
}

void AMasterTrappersAlpha1Character::Server_EndSlip_Implementation()
{
    APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
    ACharacter* character = Cast<ACharacter>(this);
    character->EnableInput(PlayerController);
    
    //GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow,"End Slip");
}

//Server Stun Functions

bool AMasterTrappersAlpha1Character::Server_StartStun_Validate()
{
        return true;
}

void AMasterTrappersAlpha1Character::Server_StartStun_Implementation()
{
    //if (Role == ROLE_Authority)
    {
        UWorld* const World = GetWorld();
        World->GetTimerManager().SetTimer(StunTimerHandle, this, &AMasterTrappersAlpha1Character::Server_EndStun, StunTimerLength, false);
        FP_PostProcessComponent->bEnabled = true;

    }
}

bool AMasterTrappersAlpha1Character::Server_EndStun_Validate()
{
  
        return true;

}

void AMasterTrappersAlpha1Character::Server_EndStun_Implementation()
{

    FP_PostProcessComponent->bEnabled = false;
}

//Client Stun Functions

void AMasterTrappersAlpha1Character::Client_StartStun_Implementation()
{
    /* if (Role == ROLE_Authority)
     {*/

    UWorld* const World = GetWorld();
    World->GetTimerManager().SetTimer(StunTimerHandle, this, &AMasterTrappersAlpha1Character::Client_EndStun, StunTimerLength, false);
    FP_PostProcessComponent->bEnabled = true;
    //}
}

bool AMasterTrappersAlpha1Character::Client_StartStun_Validate()
{
    return true;
}

void AMasterTrappersAlpha1Character::Multicast_StartStun_Implementation()//
{
   /* if (Role == ROLE_Authority)
    {*/

        UWorld* const World = GetWorld();
        World->GetTimerManager().SetTimer(StunTimerHandle, this, &AMasterTrappersAlpha1Character::Multicast_EndStun, StunTimerLength, false);
        FP_PostProcessComponent->bEnabled = true;
    //}
}

bool AMasterTrappersAlpha1Character::Client_EndStun_Validate()
{
    return true;
}

void AMasterTrappersAlpha1Character::Client_EndStun_Implementation()
{

    FP_PostProcessComponent->bEnabled = false;
}

void AMasterTrappersAlpha1Character::Multicast_EndStun_Implementation()//
{

    FP_PostProcessComponent->bEnabled = false;
}
//Multicasting Die
void AMasterTrappersAlpha1Character::Multicast_Die_Implementation()
{
    if (Role == ROLE_Authority)
    {
        HealthComponent->Server_ResetHealth();
        SetActorLocation(GetRandomRespawnLocation());
        //GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("You Died!"));
        //StartSlip();

    }
}

FVector AMasterTrappersAlpha1Character::GetRandomRespawnLocation()
{
    int randIdx = rand() % RespawnLocations.Num();
    FVector loc = RespawnLocations[randIdx];
    int bp = 1;
    return loc;
}

float AMasterTrappersAlpha1Character::GetHealth()
{
    return HealthComponent->GetPercentageHealth();
}

FQuat AMasterTrappersAlpha1Character::GetTrapSpawnRotation()
{
    //Get world
    UWorld* const World = GetWorld();
    if (World)
    {
        FActorSpawnParameters SpawnParams;
        SpawnParams.Owner = this;
        FCollisionQueryParams Params(NAME_None, FCollisionQueryParams::GetUnknownStatId());
        FHitResult HitResult;
        FTransform SpawnTransform = CursorToWorld->GetComponentTransform();

        FVector StartLocation = CursorToWorld->GetComponentLocation();
        FVector EndLocation = CursorToWorld->GetComponentRotation().Vector() * 30.0f + StartLocation;
        World->LineTraceSingleByChannel(HitResult, StartLocation, EndLocation, ECC_Visibility, Params);
        FQuat Normal = HitResult.ImpactNormal.ToOrientationRotator().Quaternion();
        Normal.RotateVector(FirstPersonCameraComponent->GetComponentRotation().Vector());
        FRotator rot = Normal.Rotator();
        rot.Pitch = rot.Pitch + 90.0f;
        rot.Normalize();
        FRotator PlayerRot = FirstPersonCameraComponent->GetComponentRotation();
        PlayerRot.Roll = 0;
        PlayerRot.Pitch = 0;
        PlayerRot.Yaw = 1;
        FQuat newQuat = rot.Quaternion() * PlayerRot.Quaternion();
        newQuat = newQuat * TrapRotation.Quaternion();
    return newQuat;
    }
    FQuat Quat = FRotator(0.f,0.f,0.f).Quaternion();
    return Quat;
}

void AMasterTrappersAlpha1Character::BeginPlay()
{
	// Call the base class  
	Super::BeginPlay();
	//Attach gun mesh component to Skeleton, doing it here because the skeleton is not yet created in the constructor
	FP_Gun->AttachToComponent(Mesh1P, FAttachmentTransformRules(EAttachmentRule::SnapToTarget, true), TEXT("GripPoint"));
    RespawnLocation = GetActorLocation();
	// Show gun mesh componen.
	Mesh1P->SetHiddenInGame(false, true);
    //if (Role < ROLE_Authority)
        Server_SetColor();
    // Display inventory after every 2 seconds
    GetWorld()->GetTimerManager().SetTimer(PostBeginPLayTimerHandle, this, &AMasterTrappersAlpha1Character::PostBeginPlay, 2.f, false);

}

bool AMasterTrappersAlpha1Character::Server_SetCursorLocation_Validate()
{
    return true;
}

void AMasterTrappersAlpha1Character::Client_SetCursorLocation()
{
    if (CursorToWorld != nullptr)
    {
        if (UWorld* World = GetWorld())
        {
            FHitResult HitResult;
            FCollisionQueryParams Params(NAME_None, FCollisionQueryParams::GetUnknownStatId());
            FVector StartLocation = FirstPersonCameraComponent->GetComponentLocation();
            FVector EndLocation = FirstPersonCameraComponent->GetComponentRotation().Vector() * 2000.0f + StartLocation;
            Params.AddIgnoredActor(this);
            World->LineTraceSingleByChannel(HitResult, StartLocation, EndLocation, ECC_Visibility, Params);
            FQuat SurfaceRotation = HitResult.ImpactNormal.ToOrientationRotator().Quaternion();
            SpawnSurfaceRotation = SurfaceRotation;
            CursorToWorld->SetWorldLocationAndRotation(HitResult.Location, SurfaceRotation);
            FRotator temp = TrapRotation;
            temp.Yaw -= 90;
            FVector trapFowardVector = temp.Vector();
            DrawDebugLine(GetWorld(), CursorToWorld->GetComponentLocation(), CursorToWorld->GetComponentLocation() + trapFowardVector * 100.0f, FColor(255, 0, 0), false, 0.1f, 0, 2.333);
        }
    }
}

void AMasterTrappersAlpha1Character::Server_SetCursorLocation_Implementation()
{
    /*if (Role == ROLE_Authority)
    {*/
        if (CursorToWorld != nullptr)
        {
            if (UWorld* World = GetWorld())
            {
                FHitResult HitResult;
                FCollisionQueryParams Params(NAME_None, FCollisionQueryParams::GetUnknownStatId());
                FVector StartLocation = FirstPersonCameraComponent->GetComponentLocation();
                FVector EndLocation = FirstPersonCameraComponent->GetComponentRotation().Vector() * 2000.0f + StartLocation;
                Params.AddIgnoredActor(this);
                World->LineTraceSingleByChannel(HitResult, StartLocation, EndLocation, ECC_Visibility, Params);
                FQuat SurfaceRotation = HitResult.ImpactNormal.ToOrientationRotator().Quaternion();
                SpawnSurfaceRotation = SurfaceRotation;
                CursorToWorld->SetWorldLocationAndRotation(HitResult.Location, SurfaceRotation);
                FRotator temp = TrapRotation;
                temp.Yaw -= 90;
                FVector trapFowardVector = temp.Vector();
                DrawDebugLine(GetWorld(), CursorToWorld->GetComponentLocation(), CursorToWorld->GetComponentLocation() + trapFowardVector * 100.0f, FColor(255, 0, 0), false, 0.1f, 0, 2.333);
            }
        }
   // }
}

void AMasterTrappersAlpha1Character::Tick(float DeltaSeconds)
{
    Super::Tick(DeltaSeconds);

    Server_SetCursorLocation();
    if (Role < ROLE_Authority)
    {
        Client_SetCursorLocation();
    }
    LocalFacingRot = FirstPersonCameraComponent->GetComponentRotation();
    LocalFacingDirection = LocalFacingRot.Vector();
    Server_ChangeFacing(LocalFacingDirection);
    SavedPosition = FirstPersonCameraComponent->GetComponentLocation();
    CurrentSpeed = GetVelocity().Size();
    if (HealthComponent->GetHealth() > 0.0f)
    {
        if (CurrentSpeed <= 0.)
        {
            HealthComponent->UpdateHealth(-0.5f);
        }
    }
    else
    {
        //Destroy();
        Multicast_Die();
        //HealthComponent->ResetHealth();
    }

    // clmap max health pickup nums
    if (CurrentHealthPickupNum >= MaxHealthPickupNum)
    {
        CurrentHealthPickupNum = MaxHealthPickupNum;
    }

    if (DroneTacticalNum < 0)
    {
        DroneTacticalNum = 0;
    }

    if (BearTrapNum < 0)
    {
        BearTrapNum = 0;
    }

    if (FlashBangNum < 0)
    {
        FlashBangNum = 0;
    }

    if (MolotovNum < 0)
    {
        MolotovNum = 0;
    }
    if (NinjaStarNum < 0)
    {
        NinjaStarNum = 0;
    }

    if (ThrowingAxeNum < 0)
    {
        ThrowingAxeNum = 0;
    }

    if (BananaPeelNum < 0)
    {
        BananaPeelNum = 0;
    }

    if (BoostTrapNum < 0)
    {
        BoostTrapNum = 0;
    }

    if (C4TrapNum < 0)
    {
        C4TrapNum = 0;
    }

    if (TripWireTrapNum < 0)
    {
        TripWireTrapNum = 0;
    }

    if (WhoopieCushionTrapNum < 0)
    {
        WhoopieCushionTrapNum = 0;
    }

    //displaying inventory
    if (GrenadeNum > 0 || CurrentHealthPickupNum>0)
    {
        Server_UpdateInventory();
    }
}

//////////////////////////////////////////////////////////////////////////
// Input

void AMasterTrappersAlpha1Character::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// set up gameplay key bindings
	check(PlayerInputComponent);

	// Bind jump events
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

	// Bind fire event
	//PlayerInputComponent->BindAction("SpawnTatical", IE_Pressed, this, &AMasterTrappersAlpha1Character::SpawnTatical);

    // Bind Shove events
    PlayerInputComponent->BindAction("Shove", IE_Pressed, this, &AMasterTrappersAlpha1Character::Shove);

    //Bind Tactical Events
    PlayerInputComponent->BindAction("SwitchTacticalUp", IE_Pressed, this, &AMasterTrappersAlpha1Character::Server_SwitchTacticalUp);
    PlayerInputComponent->BindAction("SwitchTacticalDown", IE_Pressed, this, &AMasterTrappersAlpha1Character::Server_SwitchTacticalDown);

    //Bind Trap Events
    PlayerInputComponent->BindAction("SwitchTrapUp", IE_Pressed, this, &AMasterTrappersAlpha1Character::Server_SwitchTrapUp);
    PlayerInputComponent->BindAction("SwitchTrapDown", IE_Pressed, this, &AMasterTrappersAlpha1Character::Server_SwitchTrapDown);
    PlayerInputComponent->BindAction("PlaceTrap", IE_Pressed, this, &AMasterTrappersAlpha1Character::SpawnTrap);
    PlayerInputComponent->BindAction("ActivateTrap", IE_Pressed, this, &AMasterTrappersAlpha1Character::ActivateTrap);

	// Bind movement events
	PlayerInputComponent->BindAxis("MoveForward", this, &AMasterTrappersAlpha1Character::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AMasterTrappersAlpha1Character::MoveRight);
    PlayerInputComponent->BindAxis("RotateTrap", this, &AMasterTrappersAlpha1Character::RotateTrap);

	// We have 2 versions of the rotation bindings to handle different kinds of devices differently
	// "turn" handles devices that provide an absolute delta, such as a mouse.
	// "turnrate" is for devices that we choose to treat as a rate of change, such as an analog joystick
	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("TurnRate", this, &AMasterTrappersAlpha1Character::TurnAtRate);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("LookUpRate", this, &AMasterTrappersAlpha1Character::LookUpAtRate);

    // Bind fire event
    PlayerInputComponent->BindAction("Fire", IE_Pressed, this, &AMasterTrappersAlpha1Character::SpawnTatical);    
}

bool AMasterTrappersAlpha1Character::Server_ChangeFacing_Validate(FVector TargetFacing)
{
    return true;
}

void AMasterTrappersAlpha1Character::Server_ChangeFacing_Implementation(FVector TargetFacing)
{
    ChangeFacing(TargetFacing);
}

void AMasterTrappersAlpha1Character::MoveForward(float Value)
{
	if (Value != 0.0f)
	{
		// add movement in that direction
        Value *= Speed;
		AddMovementInput(GetActorForwardVector(), Value);
	}
}

void AMasterTrappersAlpha1Character::MoveRight(float Value)
{
	if (Value != 0.0f)
	{
		// add movement in that direction
        Value *= Speed;
		AddMovementInput(GetActorRightVector(), Value);
	}
}

void AMasterTrappersAlpha1Character::ChangeFacing(FVector TargetFacing)
{
    SetActorRotation(TargetFacing.Rotation());
    Facing = TargetFacing;
    //Facing.Y = 0.0f;
    //Facing.X = 0.0f;
    //Server_ChangeFacing(Facing);
    if (Role < ROLE_Authority)
    {
        Server_ChangeFacing(Facing);
    }
}

bool AMasterTrappersAlpha1Character::Shove_Validate()
{
    return true;
}

void AMasterTrappersAlpha1Character::Shove_Implementation()
{
    //Sets shoving bool to true for collision with other players
    bIsShoving = true;
    LaunchCharacter(GetActorForwardVector()*ShoveStrength, false, false);
    TArray<AActor*> OutOverlappingActors;
    TSubclassOf<AActor> ClassFilter;
    //Check for overlapping actors
    GetOverlappingActors(OutOverlappingActors, ClassFilter);
    for (int i = 0; i < OutOverlappingActors.Num(); i++)
    {
        //Check overlapping actors for player tag
        if (OutOverlappingActors[i]->ActorHasTag("Player"))
        {
            //Cast as AMasterTrappersAlpha1Character
            AMasterTrappersAlpha1Character* otherCharacter = Cast<AMasterTrappersAlpha1Character>(OutOverlappingActors[i]);
            otherCharacter->LaunchCharacter(GetActorForwardVector()*ShoveStrength, false, false);
        }
    }
}
bool AMasterTrappersAlpha1Character::RotateTrap_Validate(float Value)
{
    return true;
}

void AMasterTrappersAlpha1Character::RotateTrap_Implementation(float Value)
{
    if (Role == ROLE_Authority)
    {
        if (Value != 0.0f)
        {
            // add movement in that direction
            TrapRotation.Yaw += Value * 90;
            if (TrapRotation.Yaw > 360.0f)
            {
                TrapRotation.Yaw = 360.0f - TrapRotation.Yaw;
            }
            FString s = FString::SanitizeFloat(TrapRotation.Yaw);
            //GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Green, TEXT("Rotation - " + s));
            //AddMovementInput(GetActorRightVector(), Value);
        }
    }
}

void AMasterTrappersAlpha1Character::TurnAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void AMasterTrappersAlpha1Character::LookUpAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}

bool AMasterTrappersAlpha1Character::SpawnTatical_Validate()
{
    return true;
}

void AMasterTrappersAlpha1Character::SpawnTatical_Implementation()
{
    /*if (Role == ROLE_Authority)
    {
     */       UWorld* const World = GetWorld();
            if (World != NULL)
            {
                const FRotator SpawnRotation = GetControlRotation(); //LocalFacingRot;//GetControlRotation()
                //GEngine->AddOnScreenDebugMessage(0, 10.5f, FColor::Yellow, *SpawnRotation.ToString());
                //const FRotator SpawnRotation = FP_MuzzleLocation->GetComponentRotation();
                // MuzzleOffset is in camera space, so transform it to world space before offsetting from the character location to find the final muzzle position
                //const FVector SpawnLocation = ((FP_MuzzleLocation != nullptr) ? FP_MuzzleLocation->GetComponentLocation() : GetActorLocation()) + SpawnRotation.RotateVector(GunOffset);
                //const FVector SpawnLocation = FP_MuzzleLocation->GetComponentLocation();
                const FVector SpawnLocation = GetActorLocation() + SpawnRotation.RotateVector(GunOffset);
                GEngine->AddOnScreenDebugMessage(0, 10.5f, FColor::Blue, *GetActorLocation().ToString());
                GEngine->AddOnScreenDebugMessage(0, 10.5f, FColor::Red, *SpawnLocation.ToString());
                //if(FP_MuzzleLocation != nullptr)

                //Set Spawn Collision Handling Override
                FActorSpawnParameters ActorSpawnParams;
                ActorSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;//AdjustIfPossibleButDontSpawnIfColliding

                // spawn the projectile at the muzzle
             if (currentTactical == 0 && GrenadeNum>0)
            {
                 AGrenadeTactical* SpawnedActor = World->SpawnActor<AGrenadeTactical>(GrenadeTactical, SpawnLocation, SpawnRotation, ActorSpawnParams);
                 if (SpawnedActor)
                 {
                     SpawnedActor->SetMaterial(CharacterMaterial);
                     SpawnedActor->Server_SetMaterial(CharacterMaterial);
                     GrenadeNum--;
                     // try and play the sound if specified
                     if (FireSound != NULL)
                     {
                         UGameplayStatics::PlaySoundAtLocation(this, FireSound, GetActorLocation());
                     }
                     SpawnedActor->Team = Team;
                     SpawnedActor->SetOwner(this);
                 }
            }
                else if (currentTactical == 1 && FlashBangNum>0)
                {
                    AFlashBangTactical* SpawnedActor = World->SpawnActor<AFlashBangTactical>(FlashBangTactical, SpawnLocation, SpawnRotation, ActorSpawnParams);
                    if (SpawnedActor)
                    {
                        SpawnedActor->SetMaterial(CharacterMaterial);
                        SpawnedActor->Server_SetMaterial(CharacterMaterial);
                        FlashBangNum--;
                        SpawnedActor->Team = Team;
                        SpawnedActor->SetOwner(this);
                        if (FireSound != NULL)
                        {
                            UGameplayStatics::PlaySoundAtLocation(this, FireSound, GetActorLocation());
                        }
                    }
                }
                else if (currentTactical == 2 && MolotovNum>0)
                {
                    AMolotovTactical* SpawnedActor = World->SpawnActor<AMolotovTactical>(MolotovTactical, SpawnLocation, SpawnRotation, ActorSpawnParams);
                    if (SpawnedActor)
                    {
                        SpawnedActor->SetMaterial(CharacterMaterial);
                        SpawnedActor->Server_SetMaterial(CharacterMaterial);
                        MolotovNum--;
                        SpawnedActor->Team = Team;
                        SpawnedActor->SetOwner(this);
                        if (FireSound != NULL)
                        {
                            UGameplayStatics::PlaySoundAtLocation(this, FireSound, GetActorLocation());
                        }
                    }
                }
                else if (currentTactical == 3 && NinjaStarNum>0)
                {
                    ANinjaStarTactical* SpawnedActor = World->SpawnActor<ANinjaStarTactical>(NinjaStarTactical, SpawnLocation, SpawnRotation, ActorSpawnParams);
                    if (SpawnedActor)
                    {
                        SpawnedActor->SetMaterial(CharacterMaterial);
                        SpawnedActor->Server_SetMaterial(CharacterMaterial);
                        NinjaStarNum--;
                        SpawnedActor->Team = Team;
                        SpawnedActor->SetOwner(this);
                        if (FireSound != NULL)
                        {
                            UGameplayStatics::PlaySoundAtLocation(this, FireSound, GetActorLocation());
                        }
                    }
                }
                else if (currentTactical == 4 && ThrowingAxeNum>0)
                {
                    AThrowingAxeTactical* SpawnedActor = World->SpawnActor<AThrowingAxeTactical>(ThrowingAxeTactical, SpawnLocation, SpawnRotation, ActorSpawnParams);
                    if (SpawnedActor)
                    {
                        SpawnedActor->SetMaterial(CharacterMaterial);
                        SpawnedActor->Server_SetMaterial(CharacterMaterial);
                        ThrowingAxeNum--;
                        SpawnedActor->Team = Team;
                        SpawnedActor->SetOwner(this);
                        if (FireSound != NULL)
                        {
                            UGameplayStatics::PlaySoundAtLocation(this, FireSound, GetActorLocation());
                        }
                    }
                }
                else if (currentTactical == 5 && DroneTacticalNum>0)
                {
                    ADroneTactical* SpawnedActor = World->SpawnActor<ADroneTactical>(DroneTactical, SpawnLocation, SpawnRotation, ActorSpawnParams);
                    if (SpawnedActor)
                    {
                        SpawnedActor->SetMaterial(CharacterMaterial);
                        SpawnedActor->Server_SetMaterial(CharacterMaterial);
                        DroneTacticalNum--;
                        SpawnedActor->Team = Team;
                        SpawnedActor->SetOwner(this);
                        if (FireSound != NULL)
                        {
                            UGameplayStatics::PlaySoundAtLocation(this, FireSound, GetActorLocation());
                        }
                    }
                }

           

            // try and play a firing animation if specified
            if (FireAnimation != NULL)
            {
                // Get the animation object for the arms mesh
                UAnimInstance* AnimInstance = Mesh1P->GetAnimInstance();
                if (AnimInstance != NULL)
                {
                    AnimInstance->Montage_Play(FireAnimation, 1.f);
                }
            }
        //}

        //else
        //{
        //    if (GrenadeNum <= 0)
        //    {
        //        FString s = "Run out of grenade, take some grenade pickups";
        //        //GEngine->AddOnScreenDebugMessage(1, 3, FColor::Green, *s);
        //    }

        //}
    }
}

void AMasterTrappersAlpha1Character::AssignTeams()
{
    if (GetLocalGameState()->bIsPlayerOneLoggedIn == false)
    {
        Team = 1;
        GetLocalGameState()->bIsPlayerOneLoggedIn = true;
        GetLocalGameState()->numberOfPlayersLoggedIn++;
    }
    else if (GetLocalGameState()->numberOfPlayersLoggedIn == 1)
    {
        Team = 2;
        GetLocalGameState()->bIsPlayerTwoLoggedIn = true;
        GetLocalGameState()->numberOfPlayersLoggedIn++;
    }
    else if (GetLocalGameState()->numberOfPlayersLoggedIn == 2)
    {
        Team = 3;
        GetLocalGameState()->numberOfPlayersLoggedIn++;
    }
    else if (GetLocalGameState()->numberOfPlayersLoggedIn == 3)
    {
        Team = 4;
        GetLocalGameState()->numberOfPlayersLoggedIn++;
    }
    else if (GetLocalGameState()->numberOfPlayersLoggedIn == 4)
    {
        Team = 5;
        GetLocalGameState()->numberOfPlayersLoggedIn++;
    }
}

void AMasterTrappersAlpha1Character::Multicast_AssignColors_Implementation()
{
    switch (Team)
    {
    case 1:
        CharacterMaterial = GetLocalGameState()->TeamOneMaterials;
        break;
    case 2:
        CharacterMaterial = GetLocalGameState()->TeamTwoMaterials;
        break;
    case 3:
        CharacterMaterial = GetLocalGameState()->TeamThreeMaterials;
        break;
    case 4:
        CharacterMaterial = GetLocalGameState()->TeamFourMaterials;
        break;
    case 5:
        CharacterMaterial = GetLocalGameState()->TeamFiveMaterials;
        break;
    }
    if (CharacterMaterial != nullptr)
    {
        //Mesh->SetMaterial(0, CharacterMaterial);
        Mesh1P->SetMaterial(0, CharacterMaterial);
        Server_SetColor();
    }
}

bool AMasterTrappersAlpha1Character::Server_SetColor_Validate()
{
    return true;
}

void AMasterTrappersAlpha1Character::Server_SetColor_Implementation()
{
    if (CharacterMaterial != nullptr)
    {
        Mesh1P->SetMaterial(0, CharacterMaterial);
    }
}

AMasterTrappersGameStateBase * AMasterTrappersAlpha1Character::GetLocalGameState()
{
    return Cast<AMasterTrappersGameStateBase>(GetWorld()->GetGameState());
}

AMasterTrappersAlpha1GameMode * AMasterTrappersAlpha1Character::GetLocalGameMode()
{
    return Cast<AMasterTrappersAlpha1GameMode>(GetWorld()->GetAuthGameMode());
}

//Replicates UPROPERTIES
void AMasterTrappersAlpha1Character::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    DOREPLIFETIME(AMasterTrappersAlpha1Character, CursorToWorld);
    DOREPLIFETIME(AMasterTrappersAlpha1Character, TrapRotation);
    DOREPLIFETIME(AMasterTrappersAlpha1Character, Facing);
    DOREPLIFETIME(AMasterTrappersAlpha1Character, CharacterMaterial);
    DOREPLIFETIME(AMasterTrappersAlpha1Character, Team);
    DOREPLIFETIME(AMasterTrappersAlpha1Character, GrenadeNum);
    DOREPLIFETIME(AMasterTrappersAlpha1Character, OnUpdateInventory);
    DOREPLIFETIME(AMasterTrappersAlpha1Character, InventoryComponent);
    DOREPLIFETIME(AMasterTrappersAlpha1Character, DroneTacticalNum);
    DOREPLIFETIME(AMasterTrappersAlpha1Character, BearTrapNum);
    DOREPLIFETIME(AMasterTrappersAlpha1Character, FlashBangNum);
    DOREPLIFETIME(AMasterTrappersAlpha1Character, MolotovNum);
    DOREPLIFETIME(AMasterTrappersAlpha1Character, NinjaStarNum);
    DOREPLIFETIME(AMasterTrappersAlpha1Character, ThrowingAxeNum);
    DOREPLIFETIME(AMasterTrappersAlpha1Character, BananaPeelNum);
    DOREPLIFETIME(AMasterTrappersAlpha1Character, BoostTrapNum);
    DOREPLIFETIME(AMasterTrappersAlpha1Character, C4TrapNum);
    DOREPLIFETIME(AMasterTrappersAlpha1Character, TripWireTrapNum);
    DOREPLIFETIME(AMasterTrappersAlpha1Character, WhoopieCushionTrapNum);
    DOREPLIFETIME(AMasterTrappersAlpha1Character, currentTactical);
    DOREPLIFETIME(AMasterTrappersAlpha1Character, currentTrap);
    DOREPLIFETIME(AMasterTrappersAlpha1Character, HealthComponent);
    DOREPLIFETIME(AMasterTrappersAlpha1Character, m_Score);
    DOREPLIFETIME(AMasterTrappersAlpha1Character, FireSound);
}

//add droneNum everytime by 5 when hit a pickup
void AMasterTrappersAlpha1Character::AddDroneTacticalNum()
{
    DroneTacticalNum += 5;

}

void AMasterTrappersAlpha1Character::Server_AddDroneTacticalNum_Implementation()
{
    if (Role == ROLE_Authority)
        AddDroneTacticalNum();
}

bool AMasterTrappersAlpha1Character::Server_AddDroneTacticalNum_Validate()
{
    return true;
}

//add grenadeNum everytime by 5 when hit a pickup
void AMasterTrappersAlpha1Character::AddGrenadeNum()
{ 
    GrenadeNum += 5;

}

void AMasterTrappersAlpha1Character::Server_AddGrenadeNum_Implementation()
{
    if(Role==ROLE_Authority)
    AddGrenadeNum();
}

bool AMasterTrappersAlpha1Character::Server_AddGrenadeNum_Validate()
{
    return true;
}


//add bear traps everytime by 3 when hit a pickup
void AMasterTrappersAlpha1Character::AddBearTrapNum()
{
    BearTrapNum += 2;

}


void AMasterTrappersAlpha1Character::Server_AddBearTrapNum_Implementation()
{
    if (Role == ROLE_Authority)
        AddBearTrapNum();
}

bool AMasterTrappersAlpha1Character::Server_AddBearTrapNum_Validate()
{
    return true;
}

//add bear traps everytime by 3 when hit a pickup
void AMasterTrappersAlpha1Character::AddFlashBangNum()
{
    FlashBangNum += 2;

}


void AMasterTrappersAlpha1Character::Server_AddFlashBangNum_Implementation()
{
    if (Role == ROLE_Authority)
        AddFlashBangNum();
}

bool AMasterTrappersAlpha1Character::Server_AddFlashBangNum_Validate()
{
    return true;
}

//add molotov traps everytime by 3 when hit a pickup
void AMasterTrappersAlpha1Character::AddMolotovNum()
{
    MolotovNum += 2;

}


void AMasterTrappersAlpha1Character::Server_AddMolotovNum_Implementation()
{
    if (Role == ROLE_Authority)
        AddMolotovNum();
}

bool AMasterTrappersAlpha1Character::Server_AddMolotovNum_Validate()
{
    return true;
}



//add ninja star traps everytime by 3 when hit a pickup
void AMasterTrappersAlpha1Character::AddNinjaStarNum()
{
    NinjaStarNum += 2;

}


void AMasterTrappersAlpha1Character::Server_AddNinjaStarNum_Implementation()
{
    if (Role == ROLE_Authority)
        AddNinjaStarNum();
}

bool AMasterTrappersAlpha1Character::Server_AddNinjaStarNum_Validate()
{
    return true;
}


//add throwing axes traps everytime by 3 when hit a pickup
void AMasterTrappersAlpha1Character::AddThrowingAxeNum()
{
    ThrowingAxeNum += 2;

}


void AMasterTrappersAlpha1Character::Server_AddThrowingAxeNum_Implementation()
{
    if (Role == ROLE_Authority)
        AddThrowingAxeNum();
}

bool AMasterTrappersAlpha1Character::Server_AddThrowingAxeNum_Validate()
{
    return true;
}


//add BananaPeel traps everytime by 3 when hit a pickup
void AMasterTrappersAlpha1Character::AddBananaPeelNum()
{
    BananaPeelNum += 2;

}


void AMasterTrappersAlpha1Character::Server_AddBananaPeelNum_Implementation()
{
    if (Role == ROLE_Authority)
        AddBananaPeelNum();
}

bool AMasterTrappersAlpha1Character::Server_AddBananaPeelNum_Validate()
{
    return true;
}


//add BoostTrap traps everytime by 3 when hit a pickup
void AMasterTrappersAlpha1Character::AddBoostTrapNum()
{
    BoostTrapNum += 2;

}


void AMasterTrappersAlpha1Character::Server_AddBoostTrapNum_Implementation()
{
    if (Role == ROLE_Authority)
        AddBoostTrapNum();
}

bool AMasterTrappersAlpha1Character::Server_AddBoostTrapNum_Validate()
{
    return true;
}


//add C4Trap everytime by 3 when hit a pickup
void AMasterTrappersAlpha1Character::AddC4TrapNum()
{
    C4TrapNum += 2;

}


void AMasterTrappersAlpha1Character::Server_AddC4TrapNum_Implementation()
{
    if (Role == ROLE_Authority)
        AddC4TrapNum();
}

bool AMasterTrappersAlpha1Character::Server_AddC4TrapNum_Validate()
{
    return true;
}


//add throwing axes traps everytime by 3 when hit a pickup
void AMasterTrappersAlpha1Character::AddTripWireTrapNum()
{
    TripWireTrapNum += 2;

}


void AMasterTrappersAlpha1Character::Server_AddTripWireTrapNum_Implementation()
{
    if (Role == ROLE_Authority)
        AddTripWireTrapNum();
}

bool AMasterTrappersAlpha1Character::Server_AddTripWireTrapNum_Validate()
{
    return true;
}

//add WhoopieCushionTrap everytime by 3 when hit a pickup
void AMasterTrappersAlpha1Character::AddWhoopieCushionTrapNum()
{
    WhoopieCushionTrapNum += 2;

}


void AMasterTrappersAlpha1Character::Server_AddWhoopieCushionTrapNum_Implementation()
{
    if (Role == ROLE_Authority)
        AddWhoopieCushionTrapNum();
}

bool AMasterTrappersAlpha1Character::Server_AddWhoopieCushionTrapNum_Validate()
{
    return true;
}