// Fill out your copyright notice in the Description page of Project Settings.


#include "A_MusicManager.h"
#include "RogueLikeMusicGameGameMode.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AA_MusicManager::AA_MusicManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	MusicComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("Audio Component"));
	MusicComponent->OnAudioPlaybackPercent.AddDynamic(this, &AA_MusicManager::GetPlaybackPercentage);
}

// Called when the game starts or when spawned
void AA_MusicManager::BeginPlay()
{
	Super::BeginPlay();
	ARogueLikeMusicGameGameMode* GameMode = Cast<ARogueLikeMusicGameGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
	GameMode->SetMusicManager(this);
	SetNewTrack();
}

void AA_MusicManager::SetNewTrack()
{
	if (MusicTracks.Num() <= 0 && AlreadyPlayedTracks.Num() <= 0)
		return;
	if (MusicTracks.Num() <= 0)
	{
		for (auto& entry : AlreadyPlayedTracks)
		{
			MusicTracks.Add(entry);
		}
	}
	float RandTrack = FMath::RandRange(0, MusicTracks.Num() - 1);
	MusicComponent->SetSound(MusicTracks[RandTrack]);
	MusicComponent->Play();
	CurrentPlayingTrack = MusicTracks[RandTrack];
	AlreadyPlayedTracks.Add(MusicTracks[RandTrack]);
	MusicTracks.RemoveAt(RandTrack);
}

void AA_MusicManager::GetPlaybackPercentage(const USoundWave* PlayingWave, const float PlaybackPercent)
{
	PlaybackTime = PlayingWave->Duration * PlaybackPercent;
}

// Called every frame
void AA_MusicManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AA_MusicManager::UpdateVisualization()
{
	//USoundVisualizationStatics::CalculateFrequencySpectrum(CurrentPlayingTrack, 0, PlaybackTime, 0.1f, Lights.Num(), Spectrums);
	if (LightSpectrums.Num() >= 0)
	{
		for (int i = 0; i < Lights.Num(); i++)
		{
			Lights[i]->Light->SetIntensity(FMath::Abs(LightSpectrums[i]) * 2000.f);
		}
	}
	//USoundVisualizationStatics::CalculateFrequencySpectrum(CurrentPlayingTrack, 0, PlaybackTime, 0.1f, Meshes.Num(), Spectrums);
	if (MeshSpectrums.Num() >= 0)
	{
		for (int i = 0; i < Meshes.Num(); i++)
		{
			FTransform Transform = Meshes[i]->Mesh->GetComponentTransform();
			FVector Scale = Transform.GetScale3D();
			Scale.Z = FMath::Abs(MeshSpectrums[i]);
			Meshes[i]->Mesh->SetWorldScale3D(Scale);
		}
	}
}