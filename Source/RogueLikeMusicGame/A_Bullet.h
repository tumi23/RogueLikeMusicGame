// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RogueLikeMusicGameProjectile.h"
#include "A_Bullet.generated.h"

/**
 * 
 */
UCLASS()
class ROGUELIKEMUSICGAME_API AA_Bullet : public ARogueLikeMusicGameProjectile
{
	GENERATED_BODY()
	
public:
	void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit) override;
};
