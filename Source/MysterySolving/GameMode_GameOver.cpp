#include "GameMode_GameOver.h" // 自作のゲームオーバー用GameModeのヘッダーをインクルード

#include "Kismet/GameplayStatics.h"             // プレイヤー取得やサウンド再生などの静的ユーティリティ
#include "GameFramework/PlayerController.h"     // プレイヤーコントローラー関連クラス
#include "GameFramework/Character.h"            // キャラクタークラス
#include "GameFramework/CharacterMovementComponent.h" // キャラクターの移動処理
#include "Components/TextBlock.h"               // ウィジェット内のテキストブロックを使うためのインクルード
#include "Internationalization/Text.h"          // テキストのローカライズ処理用（FText関連）

AGameMode_GameOver::AGameMode_GameOver()
{
    PrimaryActorTick.bCanEverTick = false; // 毎フレームのTick処理は不要なので無効化
}

void AGameMode_GameOver::BeginPlay()
{
    Super::BeginPlay(); // 親クラスのBeginPlayを呼び出す
    bIsClear = false;
    // 毎秒呼ばれるタイマーを設定（UpdateTimer関数を1秒ごとに実行）
    GetWorldTimerManager().SetTimer(countdownTimer, this, &AGameMode_GameOver::UpdateTimer, 1.0f, true);
}

void AGameMode_GameOver::UpdateTimer()
{
    APlayerController* playerController = UGameplayStatics::GetPlayerController(this, 0); // プレイヤーコントローラー取得
    if (bIsGameOver) return; // ゲームオーバーなら処理中断

    if (bIsPauseJudge) // ポーズ中なら…
    {
        if (pauseWidgetClass) // ウィジェットクラスが設定されていれば
        {
            UUserWidget* pauseWidget = CreateWidget<UUserWidget>(playerController, pauseWidgetClass); // ポーズ用ウィジェットを作成

            if (pauseWidget && !pauseWidget->IsInViewport()) // まだ画面に表示されていないなら
            {
                pauseWidget->AddToViewport(); // ビューポートに追加

                if (pauseWidget->IsInViewport()) // 表示された直後に
                {
                    pauseWidget->RemoveFromParent(); // 即削除（これはたぶん一時的な処理？）
                    pauseWidget->SetVisibility(ESlateVisibility::Collapsed); // 強制的に非表示に
                }
            }
        }

        return; // ポーズ中なのでここで処理終了
    }

    if (!playerController) return; // プレイヤー取得に失敗してたら中断

    if (!bIsClear) // まだクリアしていないなら（カウントダウン継続）
    {
        remainingTime -= 1.0f; // 残り時間を1秒減らす

        if (countdownWidgetClass && !countdownWidget) // ウィジェット未生成なら生成する
        {
            countdownWidget = CreateWidget<UUserWidget>(playerController, countdownWidgetClass); // カウントダウン用ウィジェット作成
            if (countdownWidget)
            {
                countdownWidget->AddToViewport(); // 画面に追加
            }
        }

        if (countdownWidget) // ウィジェットがあれば表示更新
        {
            int32 totalSeconds = FMath::CeilToInt(remainingTime); // 残り時間を整数に切り上げ
            int32 minutes = totalSeconds / 60; // 分に変換
            int32 seconds = totalSeconds % 60; // 秒に変換

            FString formattedTime = FString::Printf(TEXT("残り: %d:%02d"), minutes, seconds); // 分:秒の形式に整形

            UTextBlock* countdownText = Cast<UTextBlock>(countdownWidget->GetWidgetFromName(TEXT("countdown"))); // 通常表示テキスト取得
            UTextBlock* timeLimitCountdownText = Cast<UTextBlock>(countdownWidget->GetWidgetFromName(TEXT("TimeLimitCountdown"))); // 残り10秒表示用テキスト取得

            if (countdownText) // 通常のカウントダウン表示を更新
            {
                countdownText->SetText(FText::FromString(formattedTime)); // 分:秒でセット
            }

            if (remainingTime <= 10.0f) // 残り10秒以下なら
            {
                if (countdownText)
                {
                    countdownText->SetVisibility(ESlateVisibility::Collapsed); // 通常表示は非表示に
                }

                if (timeLimitCountdownText)
                {
                    timeLimitCountdownText->SetText(FText::FromString(formattedTime)); // 強調表示に更新
                    timeLimitCountdownText->SetVisibility(ESlateVisibility::Visible); // 表示する
                }
            }
        }

        if (remainingTime <= 0.0f) // 残り時間が0以下なら
        {
            HandleGameOver(); // ゲームオーバー処理を実行
        }
    }
    else // クリア済みの場合（タイマー止まった後に表示用）
    {
        if (clearTimeWidgetClass && !clearTimeWidget) // ウィジェット未生成なら
        {
            clearTimeWidget = CreateWidget<UUserWidget>(playerController, clearTimeWidgetClass); // クリアタイム用ウィジェット作成
            if (clearTimeWidget)
            {
                clearTimeWidget->AddToViewport(); // 画面に追加
            }
        }

        if (clearTimeWidget) // 表示更新
        {
            int32 totalSeconds = FMath::CeilToInt(remainingTime); // 残り時間を整数化（クリア時点）
            int32 minutes = totalSeconds / 60; // 分に変換
            int32 seconds = totalSeconds % 60; // 秒に変換

            FString formattedClearTime = FString::Printf(TEXT("クリア時: %d:%02d"), minutes, seconds); // 分:秒形式に整形

            UTextBlock* countdownViewText = Cast<UTextBlock>(clearTimeWidget->GetWidgetFromName(TEXT("ClearTimer"))); // テキスト取得
            if (countdownViewText)
            {
                countdownViewText->SetText(FText::FromString(formattedClearTime)); // 分:秒表示に更新
            }
        }
    }
}


void AGameMode_GameOver::HandleGameOver()
{
    bIsGameOver = true; // フラグを立てて多重処理を防ぐ
    GetWorldTimerManager().ClearTimer(countdownTimer); // タイマーを停止

    APlayerController* playerController = UGameplayStatics::GetPlayerController(this, 0); // プレイヤー取得
    if (!playerController || !gameOverWidgetClass) return; // どちらかが無ければ処理中断

    // GameOver用ウィジェットを生成して表示
    gameOverWidget = CreateWidget<UUserWidget>(playerController, gameOverWidgetClass);
    if (gameOverWidget)
    {
        gameOverWidget->AddToViewport(); // ビューポートに追加

        // UI入力専用モードに切替
        FInputModeUIOnly inputMode;
        inputMode.SetWidgetToFocus(gameOverWidget->TakeWidget()); // ウィジェットにフォーカス
        inputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock); // マウスロックなし
        playerController->SetInputMode(inputMode); // 入力モード適用
        playerController->bShowMouseCursor = true; // マウスカーソルを表示
    }

    // プレイヤーの操作を無効化
    playerController->DisableInput(playerController);
    APawn* playerPawn = playerController->GetPawn(); // プレイヤーのポーン取得
    if (playerPawn)
    {
        playerPawn->DisableInput(playerController); // ポーンの入力も無効化

        // 移動を完全に停止
        ACharacter* character = Cast<ACharacter>(playerPawn);
        if (character && character->GetCharacterMovement())
        {
            character->GetCharacterMovement()->StopMovementImmediately(); // 強制停止
        }
    }

    // ログ出力（デバッグ用）
    UE_LOG(LogTemp, Warning, TEXT("時間切れ！Game Over"));
}
