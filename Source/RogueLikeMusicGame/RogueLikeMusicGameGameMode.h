// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "A_AI_Spawner.h"
#include "A_Player_SpawnLocation.h"
#include "A_MusicManager.h"
#include "RogueLikeMusicGameGameMode.generated.h"

UENUM(BlueprintType)
enum class EPlayerUpgrades : uint8
{
	DoubleJump,
	Dodge,
	Sprint,
	Shockwave,
	AutoAim,
	Charm,
};

UENUM(BlueprintType)
enum class EWeaponUpgrades : uint8
{
	Ricochet,
	CriticalHit,
	BurstFire,
	FireBullets,
	FreezingBullets,
};

UENUM(BlueprintType)
enum class EWeapons : uint8
{
	Glock,
	AK47,
	Shotgun,
	RPG,
	Katana,
};

UCLASS(minimalapi)
class ARogueLikeMusicGameGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	ARogueLikeMusicGameGameMode();

public:
	void SetMusicManager(AA_MusicManager* NewManager) { MusicManager = NewManager; };
	UFUNCTION(BlueprintCallable)
		void GetSpawnLocationInLevel(ULevel* Level, FVector& SpawnLocation);
	UFUNCTION(BlueprintCallable)
		AA_AI_Spawner* GetAISpawnerInLevel(ULevel* Level);

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Music")
		AA_MusicManager* MusicManager;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Music")
		int32 CurrentLevel = 0;
};



