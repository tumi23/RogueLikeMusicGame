// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "RogueLikeMusicGameCharacter.h"
#include "RogueLikeMusicGameProjectile.h"
#include "Animation/AnimInstance.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "A_Bullet.h"
#include "A_Missile.h"
#include "TimerManager.h"
#include "GameFramework/Actor.h"
#include "GameFramework/InputSettings.h"
#include "HeadMountedDisplayFunctionLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "MotionControllerComponent.h"
#include "XRMotionControllerBase.h" // for FXRMotionControllerBase::RightHandSourceId

DEFINE_LOG_CATEGORY_STATIC(LogFPChar, Warning, All);

//////////////////////////////////////////////////////////////////////////
// ARogueLikeMusicGameCharacter

ARogueLikeMusicGameCharacter::ARogueLikeMusicGameCharacter()
{
	Force = CreateDefaultSubobject<URadialForceComponent>(TEXT("Force Component"));
	Force->SetupAttachment(RootComponent);

	JumpMaxCount = 1;

	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(55.f, 96.0f);

	// set our turn rates for input
	BaseTurnRate = 45.f;
	BaseLookUpRate = 45.f;

	// Create a CameraComponent	
	FirstPersonCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
	FirstPersonCameraComponent->SetupAttachment(GetCapsuleComponent());
	FirstPersonCameraComponent->SetRelativeLocation(FVector(-39.56f, 1.75f, 64.f)); // Position the camera
	FirstPersonCameraComponent->bUsePawnControlRotation = true;

	// Create a mesh component that will be used when being viewed from a '1st person' view (when controlling this pawn)
	Mesh1P = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("CharacterMesh1P"));
	Mesh1P->SetOnlyOwnerSee(true);
	Mesh1P->SetupAttachment(FirstPersonCameraComponent);
	Mesh1P->bCastDynamicShadow = false;
	Mesh1P->CastShadow = false;
	Mesh1P->SetRelativeRotation(FRotator(1.9f, -19.19f, 5.2f));
	Mesh1P->SetRelativeLocation(FVector(-0.5f, -4.4f, -155.7f));

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

	// Note: The ProjectileClass and the skeletal mesh/anim blueprints for Mesh1P, FP_Gun, and VR_Gun 
	// are set in the derived blueprint asset named MyCharacter to avoid direct content references in C++.

	// Create VR Controllers.
	R_MotionController = CreateDefaultSubobject<UMotionControllerComponent>(TEXT("R_MotionController"));
	R_MotionController->MotionSource = FXRMotionControllerBase::RightHandSourceId;
	R_MotionController->SetupAttachment(RootComponent);
	L_MotionController = CreateDefaultSubobject<UMotionControllerComponent>(TEXT("L_MotionController"));
	L_MotionController->SetupAttachment(RootComponent);

	// Create a gun and attach it to the right-hand VR controller.
	// Create a gun mesh component
	VR_Gun = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("VR_Gun"));
	VR_Gun->SetOnlyOwnerSee(true);			// only the owning player will see this mesh
	VR_Gun->bCastDynamicShadow = false;
	VR_Gun->CastShadow = false;
	VR_Gun->SetupAttachment(R_MotionController);
	VR_Gun->SetRelativeRotation(FRotator(0.0f, -90.0f, 0.0f));

	VR_MuzzleLocation = CreateDefaultSubobject<USceneComponent>(TEXT("VR_MuzzleLocation"));
	VR_MuzzleLocation->SetupAttachment(VR_Gun);
	VR_MuzzleLocation->SetRelativeLocation(FVector(0.000004, 53.999992, 10.000000));
	VR_MuzzleLocation->SetRelativeRotation(FRotator(0.0f, 90.0f, 0.0f));		// Counteract the rotation of the VR gun model.

	// Uncomment the following line to turn motion controllers on by default:
	//bUsingMotionControllers = true;
}

void ARogueLikeMusicGameCharacter::BeginPlay()
{
	// Call the base class  
	Super::BeginPlay();

	//Attach gun mesh component to Skeleton, doing it here because the skeleton is not yet created in the constructor
	FP_Gun->AttachToComponent(Mesh1P, FAttachmentTransformRules(EAttachmentRule::SnapToTarget, true), TEXT("GripPoint"));

	// Show or hide the two versions of the gun based on whether or not we're using motion controllers.
	if (bUsingMotionControllers)
	{
		VR_Gun->SetHiddenInGame(false, true);
		Mesh1P->SetHiddenInGame(true, true);
	}
	else
	{
		VR_Gun->SetHiddenInGame(true, true);
		Mesh1P->SetHiddenInGame(false, true);
	}
}

//////////////////////////////////////////////////////////////////////////
// Input

void ARogueLikeMusicGameCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// set up gameplay key bindings
	check(PlayerInputComponent);

	// Bind jump events
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

	// Bind fire event
	PlayerInputComponent->BindAction("Fire", IE_Pressed, this, &ARogueLikeMusicGameCharacter::OnFire);
	PlayerInputComponent->BindAction("Fire", IE_Released, this, &ARogueLikeMusicGameCharacter::StopFire);


	// Enable touchscreen input
	EnableTouchscreenMovement(PlayerInputComponent);

	PlayerInputComponent->BindAction("ResetVR", IE_Pressed, this, &ARogueLikeMusicGameCharacter::OnResetVR);

	// Bind movement events
	PlayerInputComponent->BindAxis("MoveForward", this, &ARogueLikeMusicGameCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ARogueLikeMusicGameCharacter::MoveRight);

	// We have 2 versions of the rotation bindings to handle different kinds of devices differently
	// "turn" handles devices that provide an absolute delta, such as a mouse.
	// "turnrate" is for devices that we choose to treat as a rate of change, such as an analog joystick
	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("TurnRate", this, &ARogueLikeMusicGameCharacter::TurnAtRate);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("LookUpRate", this, &ARogueLikeMusicGameCharacter::LookUpAtRate);
}

void ARogueLikeMusicGameCharacter::OnFire()
{
	// try and fire a projectile
	if (ProjectileClass != NULL)
	{
		UWorld* const World = GetWorld();
		if (World != NULL)
		{
			if (bUsingMotionControllers)
			{
				const FRotator SpawnRotation = VR_MuzzleLocation->GetComponentRotation();
				const FVector SpawnLocation = VR_MuzzleLocation->GetComponentLocation();
				World->SpawnActor<ARogueLikeMusicGameProjectile>(ProjectileClass, SpawnLocation, SpawnRotation);
			}
			else
			{
				switch (EquippedWeapon)
				{
				case EWeapons::Glock:
					FireGlock();
					break;
				case EWeapons::AK47:
					if (!bIsHoldingMouse)
					{
						bIsHoldingMouse = true;
						FireAK();
					}
					break;
				case EWeapons::Shotgun:
					FireShotgun();
					break;
				case EWeapons::RPG:
					FireRPG();
					break;
				case EWeapons::Katana:
					break;
				default:
					break;
				}
			}
		}
	}
	/*
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
	*/
}
void ARogueLikeMusicGameCharacter::StopFire()
{
	bIsHoldingMouse = false;
	GetWorldTimerManager().ClearTimer(ShootHoldTimer);
}

//Fire functions
void ARogueLikeMusicGameCharacter::FireGlock()
{
	const FRotator SpawnRotation = GetControlRotation();
	// MuzzleOffset is in camera space, so transform it to world space before offsetting from the character location to find the final muzzle position
	const FVector SpawnLocation = ((FP_MuzzleLocation != nullptr) ? FP_MuzzleLocation->GetComponentLocation() : GetActorLocation()) + SpawnRotation.RotateVector(GunOffset);

	// spawn the projectile at the muzzle
	//GetWorld()->SpawnActor<ARogueLikeMusicGameProjectile>(ProjectileClass, SpawnLocation, SpawnRotation);
	Shoot(SpawnLocation, SpawnRotation);
}

void ARogueLikeMusicGameCharacter::FireAK()
{
	const FRotator SpawnRotation = GetControlRotation();
	// MuzzleOffset is in camera space, so transform it to world space before offsetting from the character location to find the final muzzle position
	const FVector SpawnLocation = ((FP_MuzzleLocation != nullptr) ? FP_MuzzleLocation->GetComponentLocation() : GetActorLocation()) + SpawnRotation.RotateVector(GunOffset);

	// spawn the projectile at the muzzle
	//GetWorld()->SpawnActor<ARogueLikeMusicGameProjectile>(ProjectileClass, SpawnLocation, SpawnRotation);
	if (bIsHoldingMouse)
		Shoot(SpawnLocation, SpawnRotation);

	if (bIsHoldingMouse)
	{
		GetWorldTimerManager().SetTimer(ShootHoldTimer, this, &ARogueLikeMusicGameCharacter::FireAK, 0.2f * FireRateMultiplier, false);
	}
}

void ARogueLikeMusicGameCharacter::FireRPG()
{
	const FRotator SpawnRotation = GetControlRotation();
	// MuzzleOffset is in camera space, so transform it to world space before offsetting from the character location to find the final muzzle position
	const FVector SpawnLocation = ((FP_MuzzleLocation != nullptr) ? FP_MuzzleLocation->GetComponentLocation() : GetActorLocation()) + SpawnRotation.RotateVector(GunOffset);

	// spawn the projectile at the muzzle
	//GetWorld()->SpawnActor<ARogueLikeMusicGameProjectile>(ProjectileClass, SpawnLocation, SpawnRotation);
	Shoot(SpawnLocation, SpawnRotation);
}

void ARogueLikeMusicGameCharacter::FireShotgun()
{

}

void ARogueLikeMusicGameCharacter::SwingSword()
{

}

void ARogueLikeMusicGameCharacter::OnResetVR()
{
	UHeadMountedDisplayFunctionLibrary::ResetOrientationAndPosition();
}

void ARogueLikeMusicGameCharacter::BeginTouch(const ETouchIndex::Type FingerIndex, const FVector Location)
{
	if (TouchItem.bIsPressed == true)
	{
		return;
	}
	if ((FingerIndex == TouchItem.FingerIndex) && (TouchItem.bMoved == false))
	{
		OnFire();
	}
	TouchItem.bIsPressed = true;
	TouchItem.FingerIndex = FingerIndex;
	TouchItem.Location = Location;
	TouchItem.bMoved = false;
}

void ARogueLikeMusicGameCharacter::EndTouch(const ETouchIndex::Type FingerIndex, const FVector Location)
{
	if (TouchItem.bIsPressed == false)
	{
		return;
	}
	TouchItem.bIsPressed = false;
}

//Commenting this section out to be consistent with FPS BP template.
//This allows the user to turn without using the right virtual joystick

//void ARogueLikeMusicGameCharacter::TouchUpdate(const ETouchIndex::Type FingerIndex, const FVector Location)
//{
//	if ((TouchItem.bIsPressed == true) && (TouchItem.FingerIndex == FingerIndex))
//	{
//		if (TouchItem.bIsPressed)
//		{
//			if (GetWorld() != nullptr)
//			{
//				UGameViewportClient* ViewportClient = GetWorld()->GetGameViewport();
//				if (ViewportClient != nullptr)
//				{
//					FVector MoveDelta = Location - TouchItem.Location;
//					FVector2D ScreenSize;
//					ViewportClient->GetViewportSize(ScreenSize);
//					FVector2D ScaledDelta = FVector2D(MoveDelta.X, MoveDelta.Y) / ScreenSize;
//					if (FMath::Abs(ScaledDelta.X) >= 4.0 / ScreenSize.X)
//					{
//						TouchItem.bMoved = true;
//						float Value = ScaledDelta.X * BaseTurnRate;
//						AddControllerYawInput(Value);
//					}
//					if (FMath::Abs(ScaledDelta.Y) >= 4.0 / ScreenSize.Y)
//					{
//						TouchItem.bMoved = true;
//						float Value = ScaledDelta.Y * BaseTurnRate;
//						AddControllerPitchInput(Value);
//					}
//					TouchItem.Location = Location;
//				}
//				TouchItem.Location = Location;
//			}
//		}
//	}
//}

void ARogueLikeMusicGameCharacter::MoveForward(float Value)
{
	if (Value != 0.0f)
	{
		// add movement in that direction
		AddMovementInput(GetActorForwardVector(), Value);
	}
}

void ARogueLikeMusicGameCharacter::MoveRight(float Value)
{
	if (Value != 0.0f)
	{
		// add movement in that direction
		AddMovementInput(GetActorRightVector(), Value);
	}
}

void ARogueLikeMusicGameCharacter::TurnAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void ARogueLikeMusicGameCharacter::LookUpAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}

bool ARogueLikeMusicGameCharacter::EnableTouchscreenMovement(class UInputComponent* PlayerInputComponent)
{
	if (FPlatformMisc::SupportsTouchInput() || GetDefault<UInputSettings>()->bUseMouseForTouch)
	{
		PlayerInputComponent->BindTouch(EInputEvent::IE_Pressed, this, &ARogueLikeMusicGameCharacter::BeginTouch);
		PlayerInputComponent->BindTouch(EInputEvent::IE_Released, this, &ARogueLikeMusicGameCharacter::EndTouch);

		//Commenting this out to be more consistent with FPS BP template.
		//PlayerInputComponent->BindTouch(EInputEvent::IE_Repeat, this, &ARogueLikeMusicGameCharacter::TouchUpdate);
		return true;
	}
	
	return false;
}

void ARogueLikeMusicGameCharacter::UpgradePlayer(EPlayerUpgrades Upgrade)
{
	PlayerUpgradesList.Emplace(Upgrade, true);
	switch (Upgrade)
	{
	case EPlayerUpgrades::DoubleJump:
		JumpMaxCount = 2;
		break;
	case EPlayerUpgrades::Dodge:
		break;
	case EPlayerUpgrades::Sprint:
		break;
	case EPlayerUpgrades::Shockwave:
		break;
	case EPlayerUpgrades::AutoAim:
		break;
	case EPlayerUpgrades::Charm:
		break;
	default:
		break;
	}
}

void ARogueLikeMusicGameCharacter::UpgradeWeapons(EWeaponUpgrades Upgrade)
{
	WeaponUpgradesList.Emplace(Upgrade, true);
	switch (Upgrade)
	{
	case EWeaponUpgrades::Ricochet:
		break;
	case EWeaponUpgrades::CriticalHit:
		break;
	case EWeaponUpgrades::BurstFire:
		break;
	case EWeaponUpgrades::FireBullets:
		break;
	case EWeaponUpgrades::FreezingBullets:
		break;
	default:
		break;
	}
}

void ARogueLikeMusicGameCharacter::AddWeapon(EWeapons Weapon)
{
	WeaponsList.Emplace(Weapon, true);
}

void ARogueLikeMusicGameCharacter::SwitchEquippedWeapon(EWeapons Weapon)
{
	EquippedWeapon = Weapon;
	bIsHoldingMouse = false;
	switch (Weapon)
	{
	case EWeapons::Glock:
		ShownAmmo = GlockAmmo;
		break;
	case EWeapons::AK47:
		ShownAmmo = AKAmmo;
		break;
	case EWeapons::Shotgun:
		ShownAmmo = ShotgunAmmo;
		break;
	case EWeapons::RPG:
		ShownAmmo = RPGAmmo;
		break;
	case EWeapons::Katana:
		break;
	default:
		break;
	}
}

TArray<AActor*> ARogueLikeMusicGameCharacter::DoShockwave()
{
	TArray<AActor*> ReturnArray;
	FCollisionQueryParams TraceParams;
	TraceParams.bTraceComplex = true;
	//Ignore Actors
	TraceParams.AddIgnoredActor(this);
	//Re-initialize hit info
	TArray<FOverlapResult> Overlap;
	FCollisionShape collisionShape = FCollisionShape::MakeSphere(Force->Radius);

	if (GetWorld()->OverlapMultiByChannel(Overlap, GetActorLocation(), GetActorForwardVector().ToOrientationQuat(), ECC_GameTraceChannel2, collisionShape, TraceParams))
	{
		for (auto& entry : Overlap)
		{
			ReturnArray.Add(entry.GetActor());
		}
	}
	Force->FireImpulse();
	return ReturnArray;
}