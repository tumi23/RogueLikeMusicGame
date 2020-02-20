// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "RogueLikeMusicGame/RogueLikeMusicGameGameMode.h"
#include "PhysicsEngine/RadialForceComponent.h"
#include "RogueLikeMusicGameCharacter.generated.h"

class UInputComponent;

UCLASS(config=Game)
class ARogueLikeMusicGameCharacter : public ACharacter
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
	ARogueLikeMusicGameCharacter();

protected:
	virtual void BeginPlay();

public:
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
	UPROPERTY(EditAnywhere, Category=Projectile)
	TSubclassOf<class ARogueLikeMusicGameProjectile> ProjectileClass;

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

	/** Resets HMD orientation and position in VR. */
	void OnResetVR();

	/** Handles moving forward/backward */
	void MoveForward(float Val);

	/** Handles stafing movement, left and right */
	void MoveRight(float Val);

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



/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
																			// Our stuff //
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	//------------Variables------------//
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Projectile)
		URadialForceComponent* Force;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Stats)
		bool bIsHoldingMouse;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Stats)
		int PlayerHealth;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Stats)
		float FireRateMultiplier = 0.75;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Stats)
		float AmmoMultiplier = 1.5;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Stats)
		int ShownAmmo;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Stats)
		int GlockAmmo = 1000000;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Stats)
		int AKAmmo = 80;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Stats)
		int ShotgunAmmo = 20;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Stats)
		int RPGAmmo = 12;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Stats)
		int PlayerShield;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Stats)
		EWeapons EquippedWeapon = EWeapons::Glock;

	//Lists of aquired items
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TMap<EPlayerUpgrades, bool> PlayerUpgradesList;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TMap<EWeaponUpgrades, bool> WeaponUpgradesList;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TMap<EWeapons, bool> WeaponsList;


	//------------Functions------------//
public:
	UFUNCTION(BlueprintCallable, Category = Upgrades)
		void UpgradePlayer(EPlayerUpgrades Upgrade);

	UFUNCTION(BlueprintCallable, Category = Upgrades)
		void UpgradeWeapons(EWeaponUpgrades Upgrade);

	UFUNCTION(BlueprintCallable, Category = Upgrades)
		void AddWeapon(EWeapons Weapon);

	UFUNCTION(BlueprintCallable, Category = Upgrades)
		void SwitchEquippedWeapon(EWeapons Weapon);

	UFUNCTION(BlueprintCallable, Category = Upgrades)
		TArray<AActor*> DoShockwave();

	UFUNCTION(BlueprintImplementableEvent, Category = "Gun")
		void Shoot(FVector SpawnLocation, FRotator SpawnRotation);

	FTimerHandle ShootHoldTimer;

	//Fire functions
	void StopFire();

	void FireGlock();
	void FireAK();
	void FireRPG();
	void FireShotgun();
	void SwingSword();
};

