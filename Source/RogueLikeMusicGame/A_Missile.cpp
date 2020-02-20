// Fill out your copyright notice in the Description page of Project Settings.


#include "A_Missile.h"
#include "Kismet/GameplayStatics.h"
#include "Components/PrimitiveComponent.h"

AA_Missile::AA_Missile()
{
	Force = CreateDefaultSubobject<URadialForceComponent>(TEXT("Force Component"));
	Force->SetupAttachment(RootComponent);

	Damage = 5;
}

void AA_Missile::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	// Only add impulse and destroy projectile if we hit a physics
	if ((OtherActor != NULL) && (OtherActor != this) && (OtherComp != NULL))
	{
		//explosion
		TArray<AActor*> ignoredActors;

		UGameplayStatics::ApplyRadialDamage(GetWorld(), Damage, GetActorLocation(), Force->Radius, UDamageType::StaticClass(), ignoredActors, this, nullptr);
		Force->FireImpulse();

		//Destroy();
	}
}