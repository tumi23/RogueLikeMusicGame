// Fill out your copyright notice in the Description page of Project Settings.


#include "A_Player_SpawnLocation.h"

// Sets default values
AA_Player_SpawnLocation::AA_Player_SpawnLocation()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AA_Player_SpawnLocation::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AA_Player_SpawnLocation::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

