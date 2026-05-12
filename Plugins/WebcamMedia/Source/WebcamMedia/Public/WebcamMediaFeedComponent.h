// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "Components/ActorComponent.h"
#include "WebcamMediaFeedComponent.generated.h"

class UMediaPlayer;
class UMediaTexture;

/**
 * Поднимает захват с камеры (Media Framework / WMF на Win64) и подаёт кадры в UMediaTexture.
 * Можно назначить готовые ассеты Media Player / Media Texture или оставить пустыми — тогда создаются временные объекты.
 */
UCLASS(ClassGroup = (Media), meta = (BlueprintSpawnableComponent))
class UWebcamMediaFeedComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UWebcamMediaFeedComponent();

	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	/** Явный URL захвата (скопируйте из окна Media Player в редакторе). Если не пусто — перечисление устройств не используется. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Webcam")
	FString CustomCaptureUrl;

	/** Индекс устройства в списке Enumerate Video Capture Devices (0 — первое). */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Webcam", meta = (ClampMin = "0"))
	int32 DeviceIndex = 0;

	/**
	 * Битовая маска EMediaVideoCaptureDeviceFilter (-1 = все типы).
	 * Для «только веб-камеры» используйте значение флага Webcam (8).
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Webcam")
	int32 VideoCaptureDeviceFilter = -1;

	/** Автоматически открыть поток в BeginPlay. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Webcam")
	bool bAutoStart = true;

	/** Если не задано — создаётся внутренний плеер. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Webcam")
	TObjectPtr<UMediaPlayer> MediaPlayer;

	/** Если не задано — создаётся внутренняя текстура и привязывается к плееру. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Webcam")
	TObjectPtr<UMediaTexture> MediaTexture;

	/** Начать или перезапустить захват по текущим настройкам. */
	UFUNCTION(BlueprintCallable, Category = "Webcam")
	void StartWebcam();

	/** Остановить плеер и закрыть источник. */
	UFUNCTION(BlueprintCallable, Category = "Webcam")
	void StopWebcam();

	/** Последний открытый URL (для отладки). */
	UFUNCTION(BlueprintPure, Category = "Webcam")
	FString GetActiveCaptureUrl() const { return ActiveCaptureUrl; }

	/** Текущая текстура (в т.ч. созданная автоматически) — для UMG / материала. */
	UFUNCTION(BlueprintPure, Category = "Webcam")
	UMediaTexture* GetMediaTexture() const { return MediaTexture; }

	/** Текущий плеер. */
	UFUNCTION(BlueprintPure, Category = "Webcam")
	UMediaPlayer* GetMediaPlayer() const { return MediaPlayer; }

protected:
	void EnsureMediaObjects();
	void OpenFromEnumeration();

	UPROPERTY(Transient)
	TObjectPtr<UMediaPlayer> OwnedMediaPlayer;

	UPROPERTY(Transient)
	TObjectPtr<UMediaTexture> OwnedMediaTexture;

	FString ActiveCaptureUrl;
};
