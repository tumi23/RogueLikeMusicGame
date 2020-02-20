// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "RogueLikeMusicGameGameMode.h"
#include "RogueLikeMusicGameHUD.h"
#include "RogueLikeMusicGameCharacter.h"
#include "UObject/ConstructorHelpers.h"

ARogueLikeMusicGameGameMode::ARogueLikeMusicGameGameMode()
	: Super()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/FirstPersonCPP/Blueprints/FirstPersonCharacter"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;

	// use our custom HUD class
	HUDClass = ARogueLikeMusicGameHUD::StaticClass();
}

void ARogueLikeMusicGameGameMode::GetSpawnLocationInLevel(ULevel* Level, FVector& SpawnLocation)
{
	for (auto& entry : Level->Actors)
	{
		if (entry->IsA<AA_Player_SpawnLocation>())
		{
			SpawnLocation = entry->GetActorLocation();
			return;
		}
	}
	SpawnLocation = FVector::ZeroVector;
}

AA_AI_Spawner* ARogueLikeMusicGameGameMode::GetAISpawnerInLevel(ULevel* Level)
{
	for (auto& entry : Level->Actors)
	{
		if (entry->IsA<AA_AI_Spawner>())
			return Cast<AA_AI_Spawner>(entry);
	}
	return nullptr;
}