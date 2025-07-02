// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "MySaveSetting.generated.h"

/**
 * 
 */
UCLASS()
class MYSTERYSOLVING_API UMySaveSetting : public USaveGame
{
	GENERATED_BODY()
public:
    UPROPERTY(VisibleAnywhere, Category = "Settings")
    float savedSensitivity = 1.0f; // 保存された感度

    UPROPERTY(VisibleAnywhere, Category = "Settings")
    int32 savedGraphicsQuality = 2; // 保存されたグラフィック品質

    UPROPERTY(VisibleAnywhere, Category = "Settings")
    float savedMasterVolume = 1.0f; // 保存された音量
};
