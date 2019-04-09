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

	// Create a mesh component that will be used when being viewed from a '1st person' view (when controlling this pawn)
	Mesh1P = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("CharacterMesh1P"));
	Mesh1P->SetOnlyOwnerSee(true);
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
    ShoveStrength = 15005.0f;
    Speed = 3.0f;
    totalTacticals = 4;
    currentTactical = 0;
    totalTraps = 6;
    currentTrap = 0;
    
    
    CurrentSpeed = 0.0f;

    //Adding Player Tag
    Tags.Add("Player");

    //create inventory component
    InventoryComponent = CreateDefaultSubobject<UInventoryComponent>("Inventory Component");
    

    //create health component
    HealthComponent = CreateDefaultSubobject<UHealthComponent>("Health Component");
    
    //Initialize grenade number
    MaxGrenadeNum = 10;
    CurrentGrenadeNum = 0;

    //initialize health pickup
    MaxHealthPickupNum = 10;
    CurrentHealthPickupNum = 0;

    SetReplicates(true);
    SetReplicateMovement(true);


    //Add all possible spawn location
    RespawnLocations.Add(FVector(-160.0f, 2260.0f, 550.0f));
    RespawnLocations.Add(FVector(-610.0f,1260.0f,150.0f));
    RespawnLocations.Add(FVector(-250.0f,500.0f,630.0f));
    RespawnLocations.Add(FVector(20.0f,-1670.0f,170.0f));
    RespawnLocations.Add(FVector(-410.0f,-330.0f,170.0f));
}

void AMasterTrappersAlpha1Character::AddToInventory(ABasePickup * actor)
{
    InventoryComponent->AddToTacticalsInventory(actor);
    
}

//void AMasterTrappersAlpha1Character::RemoveFromInventory(ABasePickup * actor)
//{
//    _inventory.Add(actor);
//
//}

void AMasterTrappersAlpha1Character::UpdateInventory()
{
    InventoryComponent->UpdateTacticalsInventory();
    OnUpdateInventory.Broadcast(InventoryComponent->_inventory_tacticals);
}

void AMasterTrappersAlpha1Character::SwitchTacticalUp()
{
    //Switch the inventory of Tacticals to the next index 
    currentTactical++;
    if (currentTactical > totalTacticals)
    {
        currentTactical = 0;
    }
}

void AMasterTrappersAlpha1Character::SwitchTacticalDown()
{
    //Switch the inventory of traps to the next index 
    currentTactical--;
    if (currentTactical < 0)
    {
        currentTactical = totalTacticals;
    }
}

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
        if (BearTrap != nullptr)
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

                ABearTrap* SpawnedActor = World->SpawnActor<ABearTrap>(BearTrap, SpawnTransform, SpawnParams);
                //Set Rotation
                SpawnedActor->SetActorRelativeRotation(GetTrapSpawnRotation());
                //Set Color & Team
                SpawnedActor->SetMaterial(CharacterMaterial);
                SpawnedActor->SetTeam(Team);
                if (SpawnedActor)
                {
                    UE_LOG(LogTemp, Warning, TEXT("Bear Trap Spawned"));
                }
            }
        }
        break;
    case 1:
        if (BoostTrap != nullptr)
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
                //Set Rotation
                SpawnedActor->SetActorRelativeRotation(GetTrapSpawnRotation());
                //Set Color & Team
                SpawnedActor->SetMaterial(CharacterMaterial);
                SpawnedActor->SetTeam(Team);
                if (SpawnedActor)
                {
                    UE_LOG(LogTemp, Warning, TEXT("Boost Pad Trap Spawned"));
                }
            }
        }
        break;
    case 2:
        if (C4Trap != nullptr)
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
                //Set Rotation
                SpawnedActor->SetActorRelativeRotation(GetTrapSpawnRotation());
                //Set Color & Team
                SpawnedActor->SetMaterial(CharacterMaterial);
                SpawnedActor->SetTeam(Team);
                // Add C4 Trap to TArray of C4 traps already placed
                PlacedC4Traps.Add(SpawnedActor);
                if (SpawnedActor)
                {
                    UE_LOG(LogTemp, Warning, TEXT("C4 Trap Spawned"));
                }
            }
        }
        break;
    case 3:
        if (TripWireTrap != nullptr)
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
                //Set Rotation
                SpawnedActor->SetActorRelativeRotation(GetTrapSpawnRotation());
                //Set Color & Team
                SpawnedActor->SetMaterial(CharacterMaterial);
                SpawnedActor->SetTeam(Team);
                if (SpawnedActor)
                {
                    UE_LOG(LogTemp, Warning, TEXT("Trip Wire Trap Spawned"));
                }
            }
        }
        break;
    case 4:
        if (BananaPeelTrap != nullptr)
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

                ABananaPeelTrap* SpawnedActor = World->SpawnActor<ABananaPeelTrap>(BananaPeelTrap, SpawnTransform, SpawnParams);
                //Set Rotation
                SpawnedActor->SetActorRelativeRotation(GetTrapSpawnRotation());
                //Set Color & Team
                SpawnedActor->SetMaterial(CharacterMaterial);
                SpawnedActor->SetTeam(Team);
                if (SpawnedActor)
                {
                    UE_LOG(LogTemp, Warning, TEXT("Banana Peel Trap Spawned"));
                }
            }
        }
        break;
    case 5:
        if (WhoopieCushionTrap != nullptr)
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
                //Set Rotation
                SpawnedActor->SetActorRelativeRotation(GetTrapSpawnRotation());
                //Set Color & Team
                SpawnedActor->SetMaterial(CharacterMaterial);
                SpawnedActor->SetTeam(Team);
                if (SpawnedActor)
                {
                    UE_LOG(LogTemp, Warning, TEXT("Whoopie Cushion Trap Spawned"));
                }
            }
        }
        break;
    }
}
}

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

void AMasterTrappersAlpha1Character::SwitchTrapUp()
{
    //Switch the inventory of traps to the next index 
    currentTrap++;
    if (currentTrap > totalTraps)
    {
        currentTrap = 0;
    }
}

void AMasterTrappersAlpha1Character::SwitchTrapDown()
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
    ////TODO Week 7: Assign the AI character a team of -1
    //if (ActorHasTag("AICharacter"))
    //{
    //    playerTeam = -1;
    //    return;
    //}

    ////TODO Week 7: Assign Team Colors
    ////IF Role is Authority
    //if (Role == ROLE_Authority)
    //    //CALL Multicast_AssignTeamsColor()
    //    Multicast_AssignTeamsColor();
    ////ENDIF


    //make a material Instance of this master material. Do NOT do this in the constructor, only after post init.
    UMaterialInstanceDynamic* RV_MatInst = UMaterialInstanceDynamic::Create(MasterMaterialRef, this);
    //After you make your custom T2D, assign it as a texture parameter to your material
    //RV_MatInst->SetTextureParameterValue(FName("T2DParam"), YourCustomT2DRef);
    //ref copy: Texture2D'/Game/Inventory/hp.hp'
}

void AMasterTrappersAlpha1Character::StartSlip()
{
    UWorld* const World = GetWorld();
    World->GetTimerManager().SetTimer(BananaSlipTimerHandle, this, &AMasterTrappersAlpha1Character::EndSlip, BananaSlipTimerLength, false);
    APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
    ACharacter* character = Cast<ACharacter>(this);
    character->DisableInput(PlayerController);
    GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, "Start Slip");
}

void AMasterTrappersAlpha1Character::EndSlip()
{
    APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
    ACharacter* character = Cast<ACharacter>(this);
    character->EnableInput(PlayerController);
    
    GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow,"End Slip");
}

void AMasterTrappersAlpha1Character::StartStun()
{
    UWorld* const World = GetWorld();
    World->GetTimerManager().SetTimer(StunTimerHandle, this, &AMasterTrappersAlpha1Character::EndStun, StunTimerLength, false);
    FP_PostProcessComponent->bEnabled = true;
}

void AMasterTrappersAlpha1Character::EndStun()
{

    FP_PostProcessComponent->bEnabled = false;
}

void AMasterTrappersAlpha1Character::Multicast_Die_Implementation()
{
    HealthComponent->ResetHealth();
    SetActorLocation(GetRandomResponLocation());
    GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("You Died!"));
    //StartSlip();
}

FVector AMasterTrappersAlpha1Character::GetRandomResponLocation()
{
    int randIdx = FMath::RandRange(0, 4);
    FVector loc = RespawnLocations[randIdx];
    int bp = 1;
    return loc;
}

float AMasterTrappersAlpha1Character::GetHealth()
{
    return HealthComponent->GetHealth();
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

    // Display inventory after every 2 seconds
    //GetWorld()->GetTimerManager().SetTimer(PrintInventoryHandle, this, &AMasterTrappersAlpha1Character::UpdateInventory, 2.f, false);

}

bool AMasterTrappersAlpha1Character::Server_SetCursorLocation_Validate()
{
    return true;
}

void AMasterTrappersAlpha1Character::Server_SetCursorLocation_Implementation()
{
    if (Role == ROLE_Authority)
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
}

void AMasterTrappersAlpha1Character::Tick(float DeltaSeconds)
{
    Super::Tick(DeltaSeconds);

    Server_SetCursorLocation();
    Server_ChangeFacing(FirstPersonCameraComponent->GetComponentRotation().Vector());

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
    }


    // clmap max grenade nums
    if (CurrentGrenadeNum >= MaxGrenadeNum)
    {
        CurrentGrenadeNum = MaxGrenadeNum;
    }

    // clmap max health pickup nums
    if (CurrentHealthPickupNum >= MaxHealthPickupNum)
    {
        CurrentHealthPickupNum = MaxHealthPickupNum;
    }

    //displaying inventory
    if (CurrentGrenadeNum > 0 || CurrentHealthPickupNum>0)
    {
        UpdateInventory();
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
    PlayerInputComponent->BindAction("SwitchTacticalUp", IE_Pressed, this, &AMasterTrappersAlpha1Character::SwitchTacticalUp);
    PlayerInputComponent->BindAction("SwitchTacticalDown", IE_Pressed, this, &AMasterTrappersAlpha1Character::SwitchTacticalDown);

    //Bind Trap Events
    PlayerInputComponent->BindAction("SwitchTrapUp", IE_Pressed, this, &AMasterTrappersAlpha1Character::SwitchTrapUp);
    PlayerInputComponent->BindAction("SwitchTrapDown", IE_Pressed, this, &AMasterTrappersAlpha1Character::SwitchTrapDown);
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
    SetActorRotation(FVector(TargetFacing.X, TargetFacing.Y, 0.0f).Rotation());
    Facing = FVector(TargetFacing.X, TargetFacing.Y, 0.0f);
    //ChangeFacing(TargetFacing);
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
    SetActorRotation(FVector(TargetFacing.X, TargetFacing.Y, 0.0f).Rotation());
    Facing = FVector(TargetFacing.X, TargetFacing.Y, 0.0f);
    //Server_ChangeFacing(Facing);
    /*if (Role < ROLE_Authority)
    {
        Server_ChangeFacing(Facing);
    }*/
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
            GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Green, TEXT("Rotation - " + s));
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
    if (Role == ROLE_Authority)
    {
        if (GrenadeTactical != NULL && CurrentGrenadeNum > 0)
        {
            UWorld* const World = GetWorld();
            if (World != NULL)
            {
                const FRotator SpawnRotation = GetControlRotation();
                //const FRotator SpawnRotation = FP_MuzzleLocation->GetComponentRotation();
                // MuzzleOffset is in camera space, so transform it to world space before offsetting from the character location to find the final muzzle position
                const FVector SpawnLocation = ((FP_MuzzleLocation != nullptr) ? FP_MuzzleLocation->GetComponentLocation() : GetActorLocation()) + SpawnRotation.RotateVector(GunOffset);

                //Set Spawn Collision Handling Override
                FActorSpawnParameters ActorSpawnParams;
                ActorSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButDontSpawnIfColliding;

                // spawn the projectile at the muzzle
                if (currentTactical == 0)
                {
                    AGrenadeTactical* SpawnedActor = World->SpawnActor<AGrenadeTactical>(GrenadeTactical, SpawnLocation, SpawnRotation, ActorSpawnParams);
                    CurrentGrenadeNum--;
                    if (CurrentGrenadeNum == 0)
                    {
                        InventoryComponent->RemoveFromTacticalInventory();
                    }
                    else if (CurrentGrenadeNum == 5)
                    {
                        InventoryComponent->RemoveFromTacticalInventory();
                    }
                    SpawnedActor->SetMaterial(CharacterMaterial);
                }
                else if (currentTactical == 1)
                {
                    AFlashBangTactical* SpawnedActor = World->SpawnActor<AFlashBangTactical>(FlashBangTactical, SpawnLocation, SpawnRotation, ActorSpawnParams);
                    SpawnedActor->SetMaterial(CharacterMaterial);
                }
                else if (currentTactical == 2)
                {
                    AMolotovTactical* SpawnedActor = World->SpawnActor<AMolotovTactical>(MolotovTactical, SpawnLocation, SpawnRotation, ActorSpawnParams);
                    SpawnedActor->SetMaterial(CharacterMaterial);
                }
                else if (currentTactical == 3)
                {
                    ANinjaStarTactical* SpawnedActor = World->SpawnActor<ANinjaStarTactical>(NinjaStarTactical, SpawnLocation, SpawnRotation, ActorSpawnParams);
                    SpawnedActor->SetMaterial(CharacterMaterial);
                }
                else if (currentTactical == 4)
                {
                    AThrowingAxeTactical* SpawnedActor = World->SpawnActor<AThrowingAxeTactical>(ThrowingAxeTactical, SpawnLocation, SpawnRotation, ActorSpawnParams);
                    SpawnedActor->SetMaterial(CharacterMaterial);
                }
            }


            // try and play the sound if specified
            if (FireSound != NULL)
            {
                UGameplayStatics::PlaySoundAtLocation(this, FireSound, GetActorLocation());
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
        }

        else
        {
            if (CurrentGrenadeNum <= 0)
            {
                FString s = "Run out of grenade, take some grenade pickups";
                GEngine->AddOnScreenDebugMessage(1, 3, FColor::Green, *s);
            }

        }
    }
}

void AMasterTrappersAlpha1Character::AssignTeams()
{
    if (GetLocalGameState()->bIsPlayerOneLoggedIn == false)
    {
        Team = 1;
        GetLocalGameState()->bIsPlayerOneLoggedIn = true;
    }
    else if (GetLocalGameState()->bIsPlayerOneLoggedIn == true)
    {
        Team = 2;
        GetLocalGameState()->bIsPlayerTwoLoggedIn = true;
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
    Mesh1P->SetMaterial(0,CharacterMaterial);
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
    DOREPLIFETIME(AMasterTrappersAlpha1Character, Score);
}