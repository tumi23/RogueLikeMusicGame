// Fill out your copyright notice in the Description page of Project Settings.


#include "A_Mesh.h"

// Sets default values
AA_Mesh::AA_Mesh()
{
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
}