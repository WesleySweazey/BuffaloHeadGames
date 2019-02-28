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

	/** Gun mesh: 1st person view (seen only by self) */
	UPROPERTY(VisibleDefaultsOnly, Category = Mesh)
	class USkeletalMeshComponent* FP_Gun;

	/** Location on gun mesh where projectiles should spawn. */
	UPROPERTY(VisibleDefaultsOnly, Category = Mesh)
	class USceneComponent* FP_MuzzleLocation;

	/** Gun mesh: VR view (attached to the VR controller directly, no arm, just the actual gun) */
	UPROPERTY(VisibleDefaultsOnly, Category = Mesh)
	class USkeletalMeshComponent* VR_Gun;

	/** Location on VR gun mesh where projectiles should spawn. */
	UPROPERTY(VisibleDefaultsOnly, Category = Mesh)
	class USceneComponent* VR_MuzzleLocation;

	/** First person camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FirstPersonCameraComponent;

	/** Motion controller (right hand) */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class UMotionControllerComponent* R_MotionController;

	/** Motion controller (left hand) */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class UMotionControllerComponent* L_MotionController;

public:
	AMasterTrappersAlpha1Character();

    //Inventory
private:
    TArray<class APickupActor*> _inventory;
public:
    void AddToInventory(class APickupActor* actor);

    UFUNCTION(BlueprintCallable)
        void UpdateInventory();

    //BlueprintAssignable make it can be binded in the blueprint
    //UPROPERTY(BlueprintAssignable, Category = "Pickup")
        //FUpdateInventoryDelegate OnUpdateInventory;


    //Taticals

    /** Location on gun mesh where projectiles should spawn. */
    UPROPERTY(EditDefaultsOnly,
        BlueprintReadOnly,
        Category = "Taticals")
        TSubclassOf<class AGrenadeTactical> GrenadeTactical;

    //Traps
    //The inventory's current trap
    UPROPERTY(EditDefaultsOnly,
        BlueprintReadOnly,
        Category = "Traps")
        int currentTrap;
    //Total amount of traps there are
    UPROPERTY(EditDefaultsOnly,
        BlueprintReadOnly,
        Category = "Traps")
        int totalTraps;

    UPROPERTY(EditDefaultsOnly,
        BlueprintReadOnly,
        Category = "Traps")
        TSubclassOf<class ABearTrap> BearTrap;

    UPROPERTY(EditDefaultsOnly,
        BlueprintReadOnly,
        Category = "Traps")
        TSubclassOf<class ABoostTrap> BoostTrap;

    UPROPERTY(EditDefaultsOnly,
        BlueprintReadOnly,
        Category = "Traps")
        TSubclassOf<class AC4Trap> C4Trap;

    UPROPERTY(EditDefaultsOnly,
        BlueprintReadOnly,
        Category = "Traps")
        TSubclassOf<class ATripWireTrap> TripWireTrap;

    UPROPERTY(EditDefaultsOnly,
        BlueprintReadOnly,
        Category = "Traps")
        TSubclassOf<class ABananaPeelTrap> BananaPeelTrap;
    
    UPROPERTY(EditDefaultsOnly,
        BlueprintReadOnly,
        Category = "Config")
        TArray <AC4Trap*> PlacedC4Traps;

    //Spawns Trap
    void SpawnTrap();
    //Activates Placed C4 Trap
    void ActivateTrap();

    //Scrolls Through Inventory Up
    void SwitchTrapUp();
    //Scrolls Through Inventory Down
    void SwitchTrapDown();
protected:
	virtual void BeginPlay();
    // Called every frame.
    virtual void Tick(float DeltaSeconds) override;
    virtual float TakeDamage(float DamageAmount, struct FDamageEvent const & DamageEvent, class AController * EventInstigator, AActor * DamageCauser);
public:

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Shoving)
        bool bIsShoving;
    //UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated, Category = Character, Meta = (ClampMin = 0.0, UIMin = 0.0))
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Shoving)
        float ShoveStrength;
    //UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated, Category = Character, Meta = (ClampMin = 0.0, UIMin = 0.0))
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
        float Speed;
    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Traps)
        FQuat SpawnSurfaceRotation;

    //Health

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health")
        float FullHealth;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health")
        float Health;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health")
        float HealthPercentage;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health")
        float CurrentSpeed;

    float TimelineValue;

    UFUNCTION(BlueprintPure, Category = "Health")
        float GetHealth();
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

	/** Projectile class to spawn */
	UPROPERTY(EditDefaultsOnly, Category=Projectile)
	TSubclassOf<class AGrenadeTactical> ProjectileClass;

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

	/** Resets HMD orientation and position in VR. */
	void OnResetVR();

	/** Handles moving forward/backward */
	void MoveForward(float Val);

	/** Handles stafing movement, left and right */
	void MoveRight(float Val);

    /** Handles moving shove */
    void Shove();

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

	struct TouchData
	{
		TouchData() { bIsPressed = false;Location=FVector::ZeroVector;}
		bool bIsPressed;
		ETouchIndex::Type FingerIndex;
		FVector Location;
		bool bMoved;
	};
	void BeginTouch(const ETouchIndex::Type FingerIndex, const FVector Location);
	void EndTouch(const ETouchIndex::Type FingerIndex, const FVector Location);
	void TouchUpdate(const ETouchIndex::Type FingerIndex, const FVector Location);
	TouchData	TouchItem;
	
protected:
	// APawn interface
	virtual void SetupPlayerInputComponent(UInputComponent* InputComponent) override;
	// End of APawn interface

	/* 
	 * Configures input for touchscreen devices if there is a valid touch interface for doing so 
	 *
	 * @param	InputComponent	The input component pointer to bind controls to
	 * @returns true if touch controls were enabled.
	 */
	bool EnableTouchscreenMovement(UInputComponent* InputComponent);

public:
	/** Returns Mesh1P subobject **/
	FORCEINLINE class USkeletalMeshComponent* GetMesh1P() const { return Mesh1P; }
	/** Returns FirstPersonCameraComponent subobject **/
	FORCEINLINE class UCameraComponent* GetFirstPersonCameraComponent() const { return FirstPersonCameraComponent; }

};

