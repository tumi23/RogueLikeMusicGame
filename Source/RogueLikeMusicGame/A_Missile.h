// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RogueLikeMusicGameProjectile.h"
#include "PhysicsEngine/RadialForceComponent.h"
#include "A_Missile.generated.h"

/**
 * 
 */
UCLASS()
class ROGUELIKEMUSICGAME_API AA_Missile : public ARogueLikeMusicGameProjectile
{
	GENERATED_BODY()
	
public:
	AA_Missile();

public:
	UPROPERTY(EditDefaultsOnly, Category = Projectile)
		URadialForceComponent* Force;


	void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit) override;
};
