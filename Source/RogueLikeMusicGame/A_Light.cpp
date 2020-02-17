// Fill out your copyright notice in the Description page of Project Settings.


#include "A_Light.h"

// Sets default values
AA_Light::AA_Light()
{
	Light = CreateDefaultSubobject<UPointLightComponent>(TEXT("Light"));
	Light->SetLightColor(FLinearColor::MakeRandomColor());
	Light->SetAttenuationRadius(16384.f);
}