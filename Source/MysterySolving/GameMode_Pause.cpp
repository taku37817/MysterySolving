// Fill out your copyright notice in the Description page of Project Settings.


#include "GameMode_Pause.h"
#include "Kismet/GameplayStatics.h" // シーン遷移やユーティリティ関数を使うため
#include "GameFramework/PlayerController.h" // プレイヤー操作や入力切り替えなどに使用
#include "GameFramework/Character.h" // ACharacterを使うためのヘッダー
#include "GameFramework/CharacterMovementComponent.h" // UCharacterMovementComponentを使うためのヘッダー
#include "Components/TextBlock.h" //UTextBlockクラス使うためのヘッダー
#include "Internationalization/Text.h" //FTextクラスを使うためのヘッダー

// Sets default values for this component's properties

AGameMode_Pause::AGameMode_Pause()
{
    PrimaryActorTick.bCanEverTick = false; // 毎フレーム更新は不要
    AutoReceiveInput = EAutoReceiveInput::Player0;
}

void AGameMode_Pause::BeginPlay()
{
    Super::BeginPlay();
    APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0);
    if (!PC) return;

    EnableInput(PC);

    // タブキーを直接バインド（プロジェクト設定不要）
    if (!InputComponent)
    {
        InputComponent = NewObject<UInputComponent>(this, TEXT("PauseInputComponent"));
        InputComponent->RegisterComponent();
        AddInstanceComponent(InputComponent);
    }
    FInputActionBinding& TabBind = InputComponent->BindAction("PauseButton", IE_Pressed, this, &AGameMode_Pause::Pause);
    TabBind.bExecuteWhenPaused = true; // 🔥 ポーズ中も有効

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
}


void AGameMode_Pause::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}

void AGameMode_Pause::Pause()
{
    if (!bIsPaused) // ポーズ中でなければ
    {
        UGameplayStatics::SetGamePaused(GetWorld(), true); // ✅ ポーズ状態にする
        ShowPauseMenu();   // UIを表示
        bIsPaused = true;  // ポーズ状態に変更
    }
    else // ポーズ中なら
    {
        UGameplayStatics::SetGamePaused(GetWorld(), false); // ✅ ポーズ解除
        HidePauseMenu();   // UIを非表示
        bIsPaused = false; // ポーズ解除状態に変更
    }
}

void AGameMode_Pause::ShowPauseMenu()
{
    APlayerController* PlayerController = UGameplayStatics::GetPlayerController(this, 0);
    if (!pauseWidgetClass || !PlayerController) return;

    if (!pauseOverWidget)
    {
        pauseOverWidget = CreateWidget<UUserWidget>(PlayerController, pauseWidgetClass);
    }

    if (pauseOverWidget && !pauseOverWidget->IsInViewport())
    {
        pauseOverWidget->AddToViewport();

        // 💡 ここを修正！！
        FInputModeGameAndUI inputMode; // ゲーム＋UI 両方有効にする
        inputMode.SetWidgetToFocus(pauseOverWidget->TakeWidget());
        inputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);

        PlayerController->SetInputMode(inputMode);    // 入力モードを切り替える
        PlayerController->bShowMouseCursor = true;    // カーソル表示
    }
}

void AGameMode_Pause::HidePauseMenu()
{
    APlayerController* PlayerController = UGameplayStatics::GetPlayerController(this, 0); // プレイヤーコントローラ取得
    if (!PlayerController || !pauseOverWidget) return; // 取得失敗やUI未作成なら中断

    pauseOverWidget->RemoveFromParent(); // UIを非表示にする（Viewportから削除）

    FInputModeGameOnly inputMode; // ゲーム操作専用の入力モードに戻す
    PlayerController->SetInputMode(inputMode); // 入力モードをゲーム用に設定
    PlayerController->bShowMouseCursor = false; // マウスカーソルを非表示に
}