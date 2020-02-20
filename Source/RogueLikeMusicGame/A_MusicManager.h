// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/AudioComponent.h"
#include "Components/PointLightComponent.h"
#include "Components/StaticMeshComponent.h"
#include "SoundVisualizations/Classes/SoundVisualizationStatics.h"
#include "A_Light.h"
#include "A_Mesh.h"
#include "Sound/SoundWave.h"
#include "A_MusicManager.generated.h"

UCLASS()
class ROGUELIKEMUSICGAME_API AA_MusicManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AA_MusicManager();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable)
		void SetNewTrack();

	UFUNCTION(BlueprintCallable)
		void UpdateVisualization();

	UFUNCTION(BlueprintCallable)
		void RegisterMesh(AA_Mesh* NewMesh) { if (!Meshes.Contains(NewMesh)) Meshes.Add(NewMesh); };
	UFUNCTION(BlueprintCallable)
		void RegisterLight(AA_Light* NewLight) { if (!Lights.Contains(NewLight)) Lights.Add(NewLight); };
	UFUNCTION(BlueprintCallable)
		void DeRegisterMeshes(AA_Mesh* NewMesh) { Meshes.Empty(); };
	UFUNCTION(BlueprintCallable)
		void DeRegisterLights(AA_Light* NewLight) { Lights.Empty(); };

	UFUNCTION()
		void GetPlaybackPercentage(const USoundWave* PlayingWave, const float PlaybackPercent);
	UFUNCTION(BlueprintImplementableEvent)
		float GetSpectrum(TArray<float>& Spectrums);

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Audio", meta = (AllowPrivateAccess = "true"))
		class UAudioComponent* MusicComponent;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Audio", meta = (AllowPrivateAccess = "true"))
		class UAudioComponent* MusicComponent2;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Audio")
		TArray<USoundWave*> MusicTracks;
	TArray<USoundWave*> AlreadyPlayedTracks;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Audio")
		USoundWave* CurrentPlayingTrack;

	UPROPERTY(BlueprintReadWrite, Category = "Audio")
		TArray<float> MeshSpectrums;
	UPROPERTY(BlueprintReadWrite, Category = "Audio")
		TArray<float> LightSpectrums;

	UPROPERTY(BlueprintReadWrite, Category = "Objects")
		TArray<AA_Mesh*> Meshes;
	UPROPERTY(BlueprintReadWrite, Category = "Objects")
		TArray<AA_Light*> Lights;

	UPROPERTY(BlueprintReadWrite, Category = "Audio")
		float PlaybackTime = 0.f;
};
