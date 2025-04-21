// Fill out your copyright notice in the Description page of Project Settings.


#include "PauseWidget.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Character.h" // ACharacterを使うためのヘッダー
#include "GameFramework/CharacterMovementComponent.h" // UCharacterMovementComponentを使うためのヘッダー
#include "Blueprint/UserWidget.h" // ゲーム内で使う UI（UUserWidget など）を扱うため
#include "Blueprint/WidgetBlueprintLibrary.h" //UWidgetBlueprintLibrary


// void UPauseWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
// {
//     Super::NativeTick(MyGeometry, InDeltaTime);
// }

// タイトルボタンが押されたときに呼ばれる

void UPauseWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
    Super::NativeTick(MyGeometry, InDeltaTime);
}

void UPauseWidget::OnTitleButtonClicked()
{
    // "TitleLevel" という名前のレベルに遷移（タイトル画面）
    UGameplayStatics::OpenLevel(this, FName("TitleLevel"));
    UE_LOG(LogTemp, Display, TEXT("タイトルへ👌"));
}
// 再開ボタン押されたときに呼ばれる
void UPauseWidget::OnBackButtonClicked()
{
    APlayerController* playerController = UGameplayStatics::GetPlayerController(this, 0); // プレイヤーコントローラ取得
    if (playerController)
    {
        playerController->EnableInput(playerController); // プレイヤーコントローラの入力を再有効化

        APawn* playerPawn = playerController->GetPawn(); // プレイヤーのPawnを取得
        if (playerPawn)
        {
            playerPawn->EnableInput(playerController); // Pawn側でも入力を再有効化

            ACharacter* character = Cast<ACharacter>(playerPawn); // キャラクターにキャスト
            if (character && character->GetCharacterMovement()) // CharacterMovementがあるか確認
            {
                character->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking); // 歩行状態に戻す
            }
        }

        FInputModeGameOnly inputMode; // ゲーム専用の入力モードを作成
        playerController->SetInputMode(inputMode); // 入力モードをゲーム専用に切り替える
        playerController->bShowMouseCursor = false; // マウスカーソルを非表示にする
    }

    UE_LOG(LogTemp, Display, TEXT("ポーズ解除 -> ウィジェット非表示👌"));
}


// リスタートボタンが押されたときに呼ばれる
void UPauseWidget::OnRestartButtonClicked()
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

// 設定画面ボタンを押されたときに呼ばれる
void UPauseWidget::OnSettingButtonClicked()
{
    // "SetteingLevel" という名前のレベルに遷移（タイトル画面）
    UGameplayStatics::OpenLevel(this, FName("SetteingLevel"));
    UE_LOG(LogTemp, Display, TEXT("タイトルへ👌"));
}