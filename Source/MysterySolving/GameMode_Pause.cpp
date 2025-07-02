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
    PrimaryActorTick.bCanEverTick = false; // 毎フレーム Tick は不要なので無効にする
    AutoReceiveInput = EAutoReceiveInput::Player0; // プレイヤー0（ローカルプレイヤー）からの入力を受け取る
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
    // APlayerController* PlayerController = UGameplayStatics::GetPlayerController(this, 0);
    // if (UGameplayStatics::IsGamePaused(GetWorld()))
    // {
    //     if (PlayerController->WasInputKeyJustPressed(EKeys::Tab))
    //     {
    //         Pause(); // Tab押されたらPause切り替え
    //     }
    // }
}

// ポーズ状態をトグルする関数（ポーズ ⇄ ポーズ解除の切り替え）
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

// ポーズメニュー（pauseOverWidget）と必要なUIを初期化して表示し、入力モードをGameAndUIに切り替える
void AGameMode_Pause::ShowPauseMenu()
{
    APlayerController* PlayerController = UGameplayStatics::GetPlayerController(this, 0); // プレイヤーコントローラ取得
    if (!pauseWidgetClass || !PlayerController) return; // ウィジェットクラスがない、またはコントローラ取得失敗なら終了

    if (!pauseOverWidget)
    {
        pauseOverWidget = CreateWidget<UUserWidget>(PlayerController, pauseWidgetClass); // ポーズ画面UI作成
    }

    if (!settingWidget && settingWidgetClass)
    {
        settingWidget = CreateWidget<UUserWidget>(PlayerController, settingWidgetClass); // 設定画面UI作成
    }

    if (!guidWidget && guidVWidgetClass)
    {
        guidWidget = CreateWidget<UUserWidget>(PlayerController, guidVWidgetClass); // ヒント画面UI作成
    }

    if (!operationGuidWidget && operationGuidWidgetClass)
    {
        operationGuidWidget = CreateWidget<UUserWidget>(PlayerController, operationGuidWidgetClass); // 操作ガイドUI作成
    }

    if (pauseOverWidget && !pauseOverWidget->IsInViewport())
    {
        pauseOverWidget->AddToViewport(); // ポーズ画面を表示
    }

    FInputModeGameAndUI inputMode;
    inputMode.SetWidgetToFocus(nullptr); // フォーカスを渡さないことでTabキーをUIナビゲーションに奪われないようにする
    inputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock); // マウスをビューポートにロックしない
    PlayerController->SetInputMode(inputMode); // 入力モードをGame＋UIに設定
    PlayerController->bShowMouseCursor = true; // マウスカーソルを表示する
}

// 設定画面（settingWidget）を表示し、マウス操作も可能にするために入力モードを設定する
void AGameMode_Pause::SettingVisible()
{
    if (!settingWidget)
    {
        UE_LOG(LogTemp, Warning, TEXT("operationGuidWidget is null!")); // エラーログ（本当は"settingWidget is null!"が正しい）
        return;
    }

    // APlayerController* PlayerController = UGameplayStatics::GetPlayerController(this, 0); // プレイヤーコントローラ取得
    // FInputModeGameAndUI inputMode;
    // inputMode.SetWidgetToFocus(nullptr); // ウィジェットにフォーカスを与えない
    // inputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock); // マウスをロックしない
    // PlayerController->SetInputMode(inputMode); // 入力モードを設定
    // PlayerController->bShowMouseCursor = true; // カーソルを表示

    if (settingWidget && !settingWidget->IsInViewport())
    {
        settingWidget->AddToViewport(); // 設定画面を表示
    }

    settingWidget->SetVisibility(ESlateVisibility::Visible); // 設定画面を可視化
}

// ヒント画面（guidWidget）を表示する（ポーズ中にヒント確認可能）
void AGameMode_Pause::GuidVisible()
{
    if (!guidWidget)
    {
        UE_LOG(LogTemp, Warning, TEXT("operationGuidWidget is null!")); // エラーログ（実際は"guidWidget is null!"が正しい）
        return;
    }

    if (guidWidget && !guidWidget->IsInViewport())
    {
        guidWidget->AddToViewport(); // ヒント画面を表示
    }

    guidWidget->SetVisibility(ESlateVisibility::Visible); // ヒント画面を可視化
}

// 操作ガイド画面（operationGuidWidget）を表示する（ポーズ中に操作確認可能）
void AGameMode_Pause::OperationGuidVisible()
{
    if (!operationGuidWidget)
    {
        UE_LOG(LogTemp, Warning, TEXT("operationGuidWidget is null!")); // エラーログ
        return;
    }

    if (!operationGuidWidget->IsInViewport())
    {
        operationGuidWidget->AddToViewport(); // 操作ガイドを表示
    }

    operationGuidWidget->SetVisibility(ESlateVisibility::Visible); // 操作ガイドを可視化
}

// ポーズ状態を解除し、表示中の全てのポーズ用ウィジェットを非表示にし、入力モードをGameOnlyに戻す
void AGameMode_Pause::HidePauseMenu()
{
    ClearAllPauseWidgets(); // 全てのポーズ関連ウィジェットを削除

    APlayerController* PlayerController = UGameplayStatics::GetPlayerController(this, 0); // プレイヤーコントローラ取得
    if (!PlayerController) return;

    pauseOverWidget = nullptr; // ポーズ画面の参照をクリア
    settingWidget = nullptr; // 設定画面の参照をクリア
    guidWidget = nullptr; // ヒント画面の参照をクリア
    operationGuidWidget = nullptr; // 操作ガイドの参照をクリア

    FInputModeGameOnly inputMode;
    PlayerController->SetInputMode(inputMode); // 入力モードをゲーム専用に戻す
    PlayerController->bShowMouseCursor = false; // カーソルを非表示に戻す
}

// すべてのポーズ用UIウィジェット（pauseOverWidgetなど）をまとめて非表示にする
void AGameMode_Pause::ClearAllPauseWidgets()
{
    TArray<UUserWidget*> pauseWidgets = { pauseOverWidget, settingWidget, guidWidget, operationGuidWidget }; // 全UIをリスト化

    for (UUserWidget* widget : pauseWidgets)
    {
        if (widget && widget->IsInViewport())
        {
            widget->RemoveFromParent(); // 表示されているウィジェットを削除
        }
    }
}
