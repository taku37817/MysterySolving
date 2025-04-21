// Fill out your copyright notice in the Description page of Project Settings.


#include "GameOverWidget.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Character.h" // ACharacterを使うためのヘッダー
#include "GameFramework/CharacterMovementComponent.h" // UCharacterMovementComponentを使うためのヘッダー

// タイトルボタンが押されたときに呼ばれる
void UGameOverWidget::OnTitleButtonClicked()
{
    // "TitleLevel" という名前のレベルに遷移（タイトル画面）
    UGameplayStatics::OpenLevel(this, FName("TitleLevel"));
    UE_LOG(LogTemp, Display, TEXT("タイトルへ👌"));
}

// リスタートボタンが押されたときに呼ばれる
void UGameOverWidget::OnRestartButtonClicked()
{
    // 現在のレベルを取得して再読み込み(リスタート)
    FName currentLevel = *UGameplayStatics::GetCurrentLevelName(this);
    UGameplayStatics::OpenLevel(this, currentLevel);
    APlayerController* playerController = UGameplayStatics::GetPlayerController(this, 0); // プレイヤーコントローラ取得
    if (playerController)
    {
        playerController->EnableInput(playerController); // プレイヤーコントローラの入力を再有効化

        APawn* playerPawn = playerController->GetPawn(); // プレイヤーのPawnを取得
        if (playerPawn)
        {
            playerPawn->EnableInput(playerController); // Pawn側でも入力を再有効化（念のため）

            ACharacter* character = Cast<ACharacter>(playerPawn); // キャラクターにキャスト
            if (character && character->GetCharacterMovement()) // CharacterMovementがあるか確認
            {
                // キャラクターの移動モードをデフォルトに戻す
                character->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking); // 歩行状態に戻す
            }
        }
        // 入力モードをデフォルトに戻す
        FInputModeGameOnly inputMode; // ゲーム専用の入力モードを作成
        playerController->SetInputMode(inputMode); // 入力モードをゲーム専用に切り替える
        playerController->bShowMouseCursor = false; // マウスカーソルを非表示
    }
    UE_LOG(LogTemp, Display, TEXT("リスタート👌"));
}