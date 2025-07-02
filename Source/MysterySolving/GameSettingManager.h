// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "GameSettingManager.generated.h"

/**
 * 
 */
UCLASS(Blueprintable)
class MYSTERYSOLVING_API UGameSettingManager : public UObject
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	void SetSensitivity(float newSensitivity); // 感度を設定し、プレイヤーに反映する

	UFUNCTION(BlueprintCallable)
	void SetGraphicsQuality(int32 level); // グラフィック品質を設定（0〜3）

	UFUNCTION(BlueprintCallable)
	void SetMasterVolume(float volume); // マスターボリュームを設定

	UFUNCTION(BlueprintCallable)
	void SaveSettings(); // 設定をセーブスロットに保存

	UFUNCTION(BlueprintCallable)
	void LoadSettings(); // セーブスロットから設定を読み込んで反映

private:
	float sensitivity = 1.0f; // 現在の感度
	int32 graphicsQuality = 2; // 現在のグラフィック品質レベル
	float masterVolume = 1.0f; // 現在の音量

	FString saveSlotName = TEXT("GameSettings"); // セーブスロット名
	uint32 userIndex = 0; // ユーザーインデックス（基本は0）
};
