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
