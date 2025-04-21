#include "TitleOperation.h" 
#include "Kismet/GameplayStatics.h" // レベル遷移に必要
#include "GameFramework/PlayerController.h" // 入力制御に必要
#include "GameFramework/Character.h" // キャラクタークラス
#include "GameFramework/CharacterMovementComponent.h" // 移動停止処理に必要


// 毎フレーム呼ばれる処理
void UTitleOperation::NativeTick(const FGeometry& MyGeometry, float InDeltaTime) 
{
    Super::NativeTick(MyGeometry, InDeltaTime); // 親クラスの Tick 処理
    // 必要ならここでウィジェットのアニメーションや状態監視処理を書く
}

// ゲーム開始ボタンが押されたときの処理
void UTitleOperation::OnGameStartButtonClicked() 
{
    APlayerController* playerController = UGameplayStatics::GetPlayerController(this, 0); // プレイヤーコントローラ取得
    if (playerController) // プレイヤーが存在するかチェック
    {
        playerController->EnableInput(playerController); // 入力を有効化
        playerController->bShowMouseCursor = false; // マウスカーソルを非表示に

        APawn* playerPawn = playerController->GetPawn(); // プレイヤーのポーン取得
        if (playerPawn) // ポーンが存在するかチェック
        {
            playerPawn->EnableInput(playerController); // ポーンの入力も有効化

            ACharacter* character = Cast<ACharacter>(playerPawn); // キャラクターにキャスト
            if (character && character->GetCharacterMovement())   // キャラと移動コンポーネントが有効か
            {
                character->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking); // 歩行モードに戻す
            }
        }

        this->RemoveFromParent(); // UIを非表示にする（忘れずに！）
    }
    UGameplayStatics::OpenLevel(this, FName("Dungeon")); // "Dungeon" レベルに遷移する
    UE_LOG(LogTemp, Display, TEXT("ゲーム開始！")); // デバッグログ出力
}

// 設定ボタンが押されたときの処理
void UTitleOperation::OnSettingButtonClicked() 
{
    UGameplayStatics::OpenLevel(this, FName("SetteingLevel")); // "SetteingLevel" に遷移する
    UE_LOG(LogTemp, Display, TEXT("設定画面へ！")); // デバッグログ出力
}
