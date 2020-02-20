// Fill out your copyright notice in the Description page of Project Settings.


#include "A_AI_Spawner.h"

// Sets default values
AA_AI_Spawner::AA_AI_Spawner()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AA_AI_Spawner::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AA_AI_Spawner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

