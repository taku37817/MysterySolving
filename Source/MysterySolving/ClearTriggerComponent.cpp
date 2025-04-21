// Fill out your copyright notice in the Description page of Project Settings.


#include "ClearTriggerComponent.h"
#include "GameMode_GameOver.h" // ゲームの状態を管理する自作 GameMode クラス
#include "Kismet/GameplayStatics.h" // プレイヤー取得やレベル遷移、サウンド再生などのユーティリティ関数
#include "GameFramework/PlayerController.h" // プレイヤー操作（入力無効化/有効化、カーソル制御など）に必要
#include "Blueprint/UserWidget.h"  // UUserWidget を扱うために必要（UI生成、AddToViewportなど）
#include "GameFramework/Character.h" // キャラクター関連クラス（ACharacterなど）を使うために必要
#include "GameFramework/CharacterMovementComponent.h" // UCharacterMovementComponent を使うために必要


UClearTriggerComponent::UClearTriggerComponent()
{
    PrimaryComponentTick.bCanEverTick = true; // Tick 有効
}
void UClearTriggerComponent::BeginPlay()
{
    Super::BeginPlay();
    bIsCleared = false; // 初期化
}

void UClearTriggerComponent::TickComponent(float deltaTime, ELevelTick tickType, FActorComponentTickFunction* thisTickFunction)
{
    Super::TickComponent(deltaTime, tickType, thisTickFunction);
    ClearOverlapFunction(); // 毎フレームオーバーラップ判定
}

// オーバーラップ判定処理
void UClearTriggerComponent::ClearOverlapFunction()
{
    if (bIsCleared || !bronzeStatueActor) return; // 処理済みかチェック

    if (IsOverlappingActor(bronzeStatueActor)) // 銅像と重なっているか
    {
        bIsCleared = true; // フラグを立てて一度だけ処理

        AGameMode_GameOver* gameOverMode = Cast<AGameMode_GameOver>(gameModeGameOverActor);
        if (gameOverMode)
        {
            gameOverMode->bIsClear = true; // ゲームクリア状態に
            gameOverMode->bIsPauseJudge = false; // ポーズ解除
        }

        APlayerController* playerController = UGameplayStatics::GetPlayerController(this, 0);
        if (playerController)
        {
            playerController->DisableInput(playerController); // 入力を無効に

            if (clearWidgetClass)
            {
                UUserWidget* clearWidget = CreateWidget<UUserWidget>(playerController, clearWidgetClass);
                if (clearWidget)
                {
                    clearWidget->AddToViewport(); // 表示

                    FInputModeUIOnly inputMode;
                    inputMode.SetWidgetToFocus(clearWidget->TakeWidget());
                    inputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
                    playerController->SetInputMode(inputMode);
                    playerController->bShowMouseCursor = true; // カーソル表示
                }
            }
        }
        APawn* playerPawn = playerController->GetPawn(); // プレイヤーのPawnを取得
        if (playerPawn)
        {
            playerPawn->DisableInput(playerController); // Pawn側でも入力を無効化（念のため）

            ACharacter* character = Cast<ACharacter>(playerPawn); // キャラクターにキャストして
            if (character && character->GetCharacterMovement()) // CharacterMovementがあるか確認
            {
                character->GetCharacterMovement()->StopMovementImmediately(); // 移動ベクトルを強制的にゼロにして止める
            }
        }
    }
}