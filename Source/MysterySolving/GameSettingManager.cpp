// Fill out your copyright notice in the Description page of Project Settings.

#include "GameSettingManager.h"
#include "MySaveSetting.h"
#include "Engine/RendererSettings.h"
#include "Scalability.h"
#include "Engine/World.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/PlayerController.h"
#include "MysterySolvingCharacter.h" // 実際のプレイヤークラスに置き換える

#if WITH_EDITOR
#include "Editor.h"
#endif

void UGameSettingManager::SetSensitivity(float newSensitivity)
{
    sensitivity = newSensitivity; // 感度を保存

    APlayerController* pc = UGameplayStatics::GetPlayerController(GetWorld(), 0); // プレイヤーコントローラー取得
    if (pc)
    {
        AMysterySolvingCharacter* player = Cast<AMysterySolvingCharacter>(pc->GetPawn()); // プレイヤーキャラクター取得
        if (player)
        {
            player->SetSensitivity(newSensitivity); // プレイヤーに感度を反映（この関数をプレイヤーに実装）
        }
    }
}

void UGameSettingManager::SetGraphicsQuality(int32 level)
{
    graphicsQuality = level;

    // スケーラビリティ一括設定（全カテゴリに反映）
    Scalability::FQualityLevels quality;
    quality.SetFromSingleQualityLevel(level);
    Scalability::SetQualityLevels(quality);

#if WITH_EDITOR
    if (GetWorld())
    {
        Scalability::SaveState(TEXT("GameUserSettings")); // エディタ用の保存（必須ではない）
    }
#endif

    // プレイヤーコントローラーからコマンドを実行（エディタやゲームに反映させるため）
    if (APlayerController* pc = UGameplayStatics::GetPlayerController(GetWorld(), 0))
    {
        // 画質カテゴリごとの設定コマンドを実行
        pc->ConsoleCommand(FString::Printf(TEXT("r.ViewDistanceQuality %d"), level));
        pc->ConsoleCommand(FString::Printf(TEXT("r.AntiAliasingQuality %d"), level));
        pc->ConsoleCommand(FString::Printf(TEXT("r.ShadowQuality %d"), level));
        pc->ConsoleCommand(FString::Printf(TEXT("r.PostProcessQuality %d"), level));
        pc->ConsoleCommand(FString::Printf(TEXT("r.TextureQuality %d"), level));
        pc->ConsoleCommand(FString::Printf(TEXT("r.EffectsQuality %d"), level));
        pc->ConsoleCommand(FString::Printf(TEXT("r.FoliageQuality %d"), level));
        pc->ConsoleCommand(FString::Printf(TEXT("r.ShadingQuality %d"), level));
    }
}

void UGameSettingManager::SetMasterVolume(float volume)
{
    masterVolume = volume; // 音量を保存

    UGameplayStatics::SetSoundMixClassOverride(
        GetWorld(),
        nullptr, // サウンドミックス（必要なら指定）
        nullptr, // サウンドクラス（必要なら指定）
        volume, // 音量
        1.0f, // ピッチ
        0.0f, // フェード時間
        true // 即時適用
    );
}

void UGameSettingManager::SaveSettings()
{
    UMySaveSetting* saveGameInstance = Cast<UMySaveSetting>(UGameplayStatics::CreateSaveGameObject(UMySaveSetting::StaticClass())); // セーブ用インスタンス作成
    saveGameInstance->savedSensitivity = sensitivity; // 感度を保存
    saveGameInstance->savedGraphicsQuality = graphicsQuality; // グラフィック品質を保存
    saveGameInstance->savedMasterVolume = masterVolume; // 音量を保存

    UGameplayStatics::SaveGameToSlot(saveGameInstance, saveSlotName, userIndex); // スロットに保存
}

void UGameSettingManager::LoadSettings()
{
    UE_LOG(LogTemp, Warning, TEXT("LoadSettings Called!"));
    if (UGameplayStatics::DoesSaveGameExist(saveSlotName, userIndex)) // セーブデータが存在するか確認
    {
        UMySaveSetting* loadedSettings = Cast<UMySaveSetting>(UGameplayStatics::LoadGameFromSlot(saveSlotName, userIndex)); // セーブデータを読み込み
        if (loadedSettings)
        {
            sensitivity = loadedSettings->savedSensitivity;
            graphicsQuality = loadedSettings->savedGraphicsQuality;
            masterVolume = loadedSettings->savedMasterVolume;

            SetSensitivity(sensitivity);
            SetGraphicsQuality(graphicsQuality);
            SetMasterVolume(masterVolume);
        }
    }
}
