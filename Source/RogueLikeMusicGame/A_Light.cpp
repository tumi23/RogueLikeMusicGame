// Fill out your copyright notice in the Description page of Project Settings.


#include "A_Light.h"
#include "Kismet/GameplayStatics.h"
#include "TimerManager.h"
#include "RogueLikeMusicGameGameMode.h"

// Sets default values
AA_Light::AA_Light()
{
	Light = CreateDefaultSubobject<UPointLightComponent>(TEXT("Light"));
	Light->SetLightColor(FLinearColor::MakeRandomColor());
	Light->SetAttenuationRadius(16384.f);
}

void AA_Light::BeginPlay()
{
	Super::BeginPlay();
	RegisterSelf();
}

void AA_Light::RegisterSelf()
{
	ARogueLikeMusicGameGameMode* GameMode = Cast<ARogueLikeMusicGameGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
	if (GameMode && GameMode->MusicManager)
	{
		GameMode->MusicManager->RegisterLight(this);
	}
	else
	{
		FTimerHandle Timer;
		GetWorldTimerManager().SetTimer(Timer, this, &AA_Light::RegisterSelf, 0.5f, false);
	}
}