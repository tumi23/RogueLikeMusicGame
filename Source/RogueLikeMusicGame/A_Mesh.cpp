// Fill out your copyright notice in the Description page of Project Settings.


#include "A_Mesh.h"
#include "Kismet/GameplayStatics.h"
#include "TimerManager.h"
#include "RogueLikeMusicGameGameMode.h"

// Sets default values
AA_Mesh::AA_Mesh()
{
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
}

void AA_Mesh::BeginPlay()
{
	Super::BeginPlay();
	RegisterSelf();
}

void AA_Mesh::RegisterSelf()
{
	ARogueLikeMusicGameGameMode* GameMode = Cast<ARogueLikeMusicGameGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
	if(GameMode && GameMode->MusicManager)
	{
		GameMode->MusicManager->RegisterMesh(this);
	}
	else
	{
		FTimerHandle Timer;
		GetWorldTimerManager().SetTimer(Timer,this, &AA_Mesh::RegisterSelf, 0.5f, false);
	}
}