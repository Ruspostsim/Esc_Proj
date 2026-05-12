// Copyright Epic Games, Inc. All Rights Reserved.

#include "WebcamMediaFeedComponent.h"

#include "MediaPlayer.h"
#include "MediaTexture.h"
#include "Misc/MediaBlueprintFunctionLibrary.h"

DEFINE_LOG_CATEGORY_STATIC(LogWebcamMedia, Log, All);

UWebcamMediaFeedComponent::UWebcamMediaFeedComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UWebcamMediaFeedComponent::BeginPlay()
{
	Super::BeginPlay();

	if (bAutoStart)
	{
		StartWebcam();
	}
}

void UWebcamMediaFeedComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	StopWebcam();
	Super::EndPlay(EndPlayReason);
}

void UWebcamMediaFeedComponent::EnsureMediaObjects()
{
	if (!MediaPlayer)
	{
		if (!OwnedMediaPlayer)
		{
			OwnedMediaPlayer = NewObject<UMediaPlayer>(this, NAME_None, RF_Transient);
		}
		MediaPlayer = OwnedMediaPlayer;
	}

	if (!MediaTexture)
	{
		if (!OwnedMediaTexture)
		{
			OwnedMediaTexture = NewObject<UMediaTexture>(this, NAME_None, RF_Transient);
			OwnedMediaTexture->SetMediaPlayer(MediaPlayer);
			OwnedMediaTexture->UpdateResource();
		}
		MediaTexture = OwnedMediaTexture;
	}
	else if (MediaTexture->GetMediaPlayer() != MediaPlayer)
	{
		MediaTexture->SetMediaPlayer(MediaPlayer);
		MediaTexture->UpdateResource();
	}
}

void UWebcamMediaFeedComponent::OpenFromEnumeration()
{
	TArray<FMediaCaptureDevice> Devices;
	UMediaBlueprintFunctionLibrary::EnumerateVideoCaptureDevices(Devices, VideoCaptureDeviceFilter);

	if (!Devices.IsValidIndex(DeviceIndex))
	{
		UE_LOG(LogWebcamMedia, Warning, TEXT("Webcam: нет устройства с индексом %d (всего %d). Проверьте камеру и VideoCaptureDeviceFilter."),
			DeviceIndex, Devices.Num());
		for (int32 i = 0; i < Devices.Num(); ++i)
		{
			UE_LOG(LogWebcamMedia, Log, TEXT("  [%d] %s -> %s"), i, *Devices[i].DisplayName.ToString(), *Devices[i].Url);
		}
		return;
	}

	ActiveCaptureUrl = Devices[DeviceIndex].Url;
	MediaPlayer->SetLooping(true);
	if (MediaPlayer->OpenUrl(ActiveCaptureUrl))
	{
		MediaPlayer->Play();
		UE_LOG(LogWebcamMedia, Log, TEXT("Webcam: открыт %s"), *ActiveCaptureUrl);
	}
	else
	{
		UE_LOG(LogWebcamMedia, Error, TEXT("Webcam: OpenUrl не удался для %s"), *ActiveCaptureUrl);
	}
}

void UWebcamMediaFeedComponent::StartWebcam()
{
	StopWebcam();
	EnsureMediaObjects();
	if (!MediaPlayer)
	{
		return;
	}

	if (!CustomCaptureUrl.IsEmpty())
	{
		ActiveCaptureUrl = CustomCaptureUrl;
		MediaPlayer->SetLooping(true);
		if (MediaPlayer->OpenUrl(ActiveCaptureUrl))
		{
			MediaPlayer->Play();
			UE_LOG(LogWebcamMedia, Log, TEXT("Webcam: открыт пользовательский URL"));
		}
		else
		{
			UE_LOG(LogWebcamMedia, Error, TEXT("Webcam: не удалось открыть CustomCaptureUrl"));
		}
		return;
	}

	OpenFromEnumeration();
}

void UWebcamMediaFeedComponent::StopWebcam()
{
	if (MediaPlayer)
	{
		MediaPlayer->Close();
	}
	ActiveCaptureUrl.Empty();
}
