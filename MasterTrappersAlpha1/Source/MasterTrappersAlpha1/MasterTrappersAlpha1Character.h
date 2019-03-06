// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "MasterTrappersAlpha1Character.generated.h"

//inventory : when the inventory get updated this event be fired
//DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FUpdateInventoryDelegate, const TArray<APickupActor*>&, InventoryItems);

class UInputComponent;

UCLASS(config=Game)
class AMasterTrappersAlpha1Character : public ACharacter
{
	GENERATED_BODY()

	/** Pawn mesh: 1st person view (arms; seen only by self) */
	UPROPERTY(VisibleDefaultsOnly, Category=Mesh)
	class USkeletalMeshComponent* Mesh1P;

    /** Pawn mesh: 1st person view (arms; seen only by self) */
    UPROPERTY(EditAnywhere, Category = Camera)
        class UPostProcessComponent* FP_PostProcessComponent;

	/** Gun mesh: 1st person view (seen only by self) */
	UPROPERTY(VisibleDefaultsOnly, Category = Mesh)
	class USkeletalMeshComponent* FP_Gun;

	/** Location on gun mesh where projectiles should spawn. */
	UPROPERTY(EditAnywhere, Category = Mesh)
	class USceneComponent* FP_MuzzleLocation;

	/** First person camera */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FirstPersonCameraComponent;

public:
    /** Character constructor */
	AMasterTrappersAlpha1Character();

    //Inventory
private:
    /** Inventory of Pick up Actors */
    TArray<class APickupActor*> _inventory;

public:
    /** Add item to inventory*/
    void AddToInventory(class APickupActor* actor);

    /** Update inventory*/
    UFUNCTION(BlueprintCallable)
        void UpdateInventory();

    //BlueprintAssignable make it can be binded in the blueprint
    //UPROPERTY(BlueprintAssignable, Category = "Pickup")
        //FUpdateInventoryDelegate OnUpdateInventory;

    //Tacticals

    /** The inventory's current tacticals index*/
    UPROPERTY(EditDefaultsOnly,
        BlueprintReadOnly,
        Category = "Tacticals")
        int currentTactical;

    /** Total amount of tacticals there are */
    UPROPERTY(EditDefaultsOnly,
        BlueprintReadOnly,
        Category = "Tacticals")
        int totalTacticals;

    /** Grenade Blueprint */
    UPROPERTY(EditDefaultsOnly,
        BlueprintReadOnly,
        Category = "Tacticals")
        TSubclassOf<class AGrenadeTactical> GrenadeTactical;

    /** Flash Bang Blueprint */
    UPROPERTY(EditDefaultsOnly,
        BlueprintReadOnly,
        Category = "Tacticals")
        TSubclassOf<class AFlashBangTactical> FlashBangTactical;

    //Scrolls Through Tactical Inventory Up
    void SwitchTacticalUp();
    //Scrolls Through Tactical Inventory Down
    void SwitchTacticalDown();

    //Traps

    /** The inventory's current trap index*/
    UPROPERTY(EditDefaultsOnly,
        BlueprintReadOnly,
        Category = "Traps")
        int currentTrap;

    /** Total amount of traps there are */
    UPROPERTY(EditDefaultsOnly,
        BlueprintReadOnly,
        Category = "Traps")
        int totalTraps;

    /** Bear trap blueprint */
    UPROPERTY(EditDefaultsOnly,
        BlueprintReadOnly,
        Category = "Traps")
        TSubclassOf<class ABearTrap> BearTrap;

    /** Boost trap blueprint */
    UPROPERTY(EditDefaultsOnly,
        BlueprintReadOnly,
        Category = "Traps")
        TSubclassOf<class ABoostTrap> BoostTrap;

    /** C4 trap blueprint */
    UPROPERTY(EditDefaultsOnly,
        BlueprintReadOnly,
        Category = "Traps")
        TSubclassOf<class AC4Trap> C4Trap;

    /** Trip Wire trap blueprint */
    UPROPERTY(EditDefaultsOnly,
        BlueprintReadOnly,
        Category = "Traps")
        TSubclassOf<class ATripWireTrap> TripWireTrap;

    /** Banana peel blueprint */
    UPROPERTY(EditDefaultsOnly,
        BlueprintReadOnly,
        Category = "Traps")
        TSubclassOf<class ABananaPeelTrap> BananaPeelTrap;
    
    /** TArray of C4 traps placed */
    UPROPERTY(EditDefaultsOnly,
        BlueprintReadOnly,
        Category = "Traps")
        TArray <AC4Trap*> PlacedC4Traps;

    /** FTimerHandle for when the player slips */
    UPROPERTY(EditDefaultsOnly,
        BlueprintReadOnly,
        Category = "Traps")
    FTimerHandle BananaSlipTimerHandle;

    /** FTimerHandle for when the player stunned */
    UPROPERTY(EditDefaultsOnly,
        BlueprintReadOnly,
        Category = "Tacticals")
        FTimerHandle StunTimerHandle;

    /** Float for how long the player slips */
    UPROPERTY(EditDefaultsOnly,
        BlueprintReadOnly,
        Category = "Traps")
    float BananaSlipTimerLength = 2.0f;

    /** Float for how long the player is stunned */
    UPROPERTY(EditDefaultsOnly,
        BlueprintReadOnly,
        Category = "Tacticals")
        float StunTimerLength = 2.0f;

    //Spawns Trap
    void SpawnTrap();
    //Activates Placed C4 Trap
    void ActivateTrap();

    //Scrolls Through Trap Inventory Up
    void SwitchTrapUp();
    //Scrolls Through Trap Inventory Down
    void SwitchTrapDown();

    //Scrolls Through Inventory Down
    void StartSlip();
    //Scrolls Through Inventory Down
    void EndSlip();

    //Scrolls Through Inventory Down
    void StartStun();
    //Scrolls Through Inventory Down
    void EndStun();

    //Scrolls Through Inventory Down
    void Die();

    //Respawn Point
    FVector RespawnLocation;
protected:
    // Called at the Beginning
	virtual void BeginPlay();
    // Called every frame.
    virtual void Tick(float DeltaSeconds) override;

    /** Takes damage if player movement is 0 */
    virtual float TakeDamage(float DamageAmount, struct FDamageEvent const & DamageEvent, class AController * EventInstigator, AActor * DamageCauser);
public:
    /** Boolean for shoving feature */
    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Shoving)
        bool bIsShoving;
    /** Boolean for shoving feature */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Shoving)
        float ShoveStrength;
    /** Float for character movement speed */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
        float Speed;
    /** Quaternion for cursor surface rotation */
    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Traps)
        FQuat SpawnSurfaceRotation;
    /** Quaternion for cursor surface rotation */
    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Traps)
        FRotator TrapRotation;
    //Health

    /** Float for maximum amount of health */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health")
        float FullHealth;
    /** Float for current amount of health */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health")
        float Health;
    /** Float for current health percent*/
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health")
        float HealthPercentage;
    /** Float for current movement speed */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health")
        float CurrentSpeed;

    /** Returns player's current health */
    UFUNCTION(BlueprintPure, Category = "Health")
        float GetHealth();
    /** Updates player's health */
    UFUNCTION(BlueprintCallable, Category = "Health")
        void UpdateHealth(float HealthChange);

    /** A decal that projects to the cursor location. */
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
        class UDecalComponent* CursorToWorld;

	/** Base turn rate, in deg/sec. Other scaling may affect final turn rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	float BaseTurnRate;

	/** Base look up/down rate, in deg/sec. Other scaling may affect final rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	float BaseLookUpRate;

	/** Gun muzzle's offset from the characters location */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Gameplay)
	FVector GunOffset;

	/** Sound to play each time we fire */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Gameplay)
	class USoundBase* FireSound;

	/** AnimMontage to play each time we fire */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	class UAnimMontage* FireAnimation;

	/** Whether to use motion controller location for aiming. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	uint32 bUsingMotionControllers : 1;
    
protected:
	
    /** Fires a projectile. */
    void OnFire();

	/** Spawn a Tatical. */
	void SpawnTatical();

	/** Handles moving forward/backward */
	void MoveForward(float Val);

	/** Handles stafing movement, left and right */
	void MoveRight(float Val);

    /** Handles moving shove */
    void Shove();

    void RotateTrap(float val);
	/**
	 * Called via input to turn at a given rate.
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void TurnAtRate(float Rate);

	/**
	 * Called via input to turn look up/down at a given rate.
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void LookUpAtRate(float Rate);

protected:
	// APawn interface
	virtual void SetupPlayerInputComponent(UInputComponent* InputComponent) override;
	// End of APawn interface

public:
	/** Returns Mesh1P subobject **/
	FORCEINLINE class USkeletalMeshComponent* GetMesh1P() const { return Mesh1P; }
	/** Returns FirstPersonCameraComponent subobject **/
	FORCEINLINE class UCameraComponent* GetFirstPersonCameraComponent() const { return FirstPersonCameraComponent; }

};

