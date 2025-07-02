// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h" // AActorを継承
#include "Blueprint/UserWidget.h" // ゲーム内で使う UI（UUserWidget など）を扱うため
#include "Kismet/GameplayStatics.h" // UGameplayStatics を使うため
#include "Sound/SoundBase.h"      // USoundBaseを使うため (SoundCueやSoundWaveの親クラス)
#include "Components/SceneComponent.h" // USceneComponent を使うため (RootComponentとして利用)
#include "Components/AudioComponent.h"          //オーディオ関連を利用に必要

#include "GameMode_GameOver.generated.h"

/**
 * 
 */
UCLASS()
class MYSTERYSOLVING_API AGameMode_GameOver :  public AActor
{
	GENERATED_BODY()

public:
    AGameMode_GameOver();
   
    UPROPERTY(BlueprintReadWrite)
    bool bIsPauseJudge = false;

    bool bIsClear = false; //クリアしたか

protected:
    virtual void BeginPlay() override; // ゲーム開始時に呼ばれる

    // UPROPERTY(EditAnywhere)
    // AActor* gameMode_PauseActor; //AGameMode_Pauseクラスのアクター

    /** タイマー用ハンドル */
    FTimerHandle countdownTimer;

    /** 残り時間（秒） */
    UPROPERTY(EditAnywhere, Category = "Timer")
    float remainingTime = 180.0f;

    /** ゲームオーバーフラグ */
    bool bIsGameOver = false;

    bool bHasPlayedTimeLimitSound = false; // BGM無限に再生の実行しないようにする


    /** ゲームオーバー時に表示するUIのクラス */
    UPROPERTY(EditAnywhere, Category = "UI")
    TSubclassOf<UUserWidget> gameOverWidgetClass;

    //カウントダウンの数値表示するUI
    UPROPERTY(EditAnywhere, Category = "UI")
    TSubclassOf<UUserWidget> countdownWidgetClass;

    //PauseWidgetクラスを使用するUIクラス
    UPROPERTY(EditAnywhere, Category = "UI")
    TSubclassOf<UUserWidget> pauseWidgetClass;

    //clearTimeWidgetのカウントを取得
    UPROPERTY(EditAnywhere, Category = "UI")
    TSubclassOf<UUserWidget> clearTimeWidgetClass;

    /** ゲームオーバーUIのインスタンス */
    UUserWidget* gameOverWidget;

    //カウントダウンUIのインスタンス
    UUserWidget* countdownWidget;

    //クリアータイムUIのインスタンス
    UUserWidget* clearTimeWidget;

      /** BGM再生用のAudioComponent */
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Audio", meta = (AllowPrivateAccess = "true"))
    UAudioComponent* myTimeLimitAudioComponent;

    /** エディタで設定するBGM */
    UPROPERTY(EditAnywhere, Category = "Audio")
    USoundBase* myTimeLimitSound;

    /** 毎秒呼ばれるタイマー関数 */
    void UpdateTimer();

    /** 時間切れ時の処理 */
    void HandleGameOver();
};