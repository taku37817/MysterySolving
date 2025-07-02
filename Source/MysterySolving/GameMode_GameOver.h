// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Blueprint/UserWidget.h" // ゲーム内で使う UI（UUserWidget など）を扱うため

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

    /** 毎秒呼ばれるタイマー関数 */
    void UpdateTimer();

    /** 時間切れ時の処理 */
    void HandleGameOver();
};