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

	myClearIntroAudioComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("MyClearIntroAudioComponent"));
	if (myClearIntroAudioComponent)
	{
		myClearIntroAudioComponent->SetupAttachment(this);
		myClearIntroAudioComponent->bAutoActivate = false; // 勝手に再生しない
	}

    myClearLoopAudioComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("MyClearLoopAudioComponent"));
	if (myClearLoopAudioComponent)
	{
		myClearLoopAudioComponent->SetupAttachment(this);
		myClearLoopAudioComponent->bAutoActivate = false; // 勝手に再生しない
	}
}
void UClearTriggerComponent::BeginPlay()
{
    Super::BeginPlay();
    bIsCleared = false; // 初期化
    if (myClearIntroAudioComponent && GetOwner() && GetOwner()->GetRootComponent())
    {
        myClearIntroAudioComponent->AttachToComponent(GetOwner()->GetRootComponent(), FAttachmentTransformRules::KeepRelativeTransform);
    }
    if (myClearLoopAudioComponent && GetOwner() && GetOwner()->GetRootComponent())
    {
        myClearLoopAudioComponent->AttachToComponent(GetOwner()->GetRootComponent(), FAttachmentTransformRules::KeepRelativeTransform);
    }

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
        // --- 音声制御処理 ---

        /*クリアBGMのイントロ*/
        if (myClearIntroAudioComponent && myClearIntroSound && !myClearIntroAudioComponent->IsPlaying())
        {
            myClearIntroAudioComponent->SetSound(myClearIntroSound);
            myClearIntroAudioComponent->SetVolumeMultiplier(1.0f);
            myClearIntroAudioComponent->bOverrideAttenuation = true;
            myClearIntroAudioComponent->AttenuationOverrides.bAttenuate = false;
            myClearIntroAudioComponent->Play(); // 再生開始
            // イントロの再生時間を取得
            float IntroDuration = myClearIntroSound->GetDuration(); // 例：5.73秒など

            // IntroDuration秒後にループを再生するタイマーをセット
            GetWorld()->GetTimerManager().SetTimer(
                loopStartTimerHandle,                   // タイマーのハンドル（解除にも使える）
                this,                                   // 対象（このクラスのインスタンス）
                &UClearTriggerComponent::SwitchFromIntroToLoop, // 呼び出す関数
                IntroDuration,                          // 待機時間（イントロの長さ）
                false                                   // ループ再実行しない（1回だけ）
            );

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

void UClearTriggerComponent::SwitchFromIntroToLoop()
{
    // イントロ停止
    if (myClearIntroAudioComponent && myClearIntroAudioComponent->IsPlaying())
    {
        myClearIntroAudioComponent->Stop();
    }
    //ループ開始
    if (myClearLoopAudioComponent && myClearLoopSound)
    {
        myClearLoopAudioComponent->SetSound(myClearLoopSound); // ループ用の音声を設定
        myClearLoopAudioComponent->SetVolumeMultiplier(1.0f);  // 音量最大
        myClearLoopAudioComponent->bOverrideAttenuation = true; // 減衰設定を上書き
        myClearLoopAudioComponent->AttenuationOverrides.bAttenuate = false; // 減衰無効
        myClearLoopAudioComponent->Play(); // 再生開始

        UE_LOG(LogTemp, Warning, TEXT("🎵 イントロ終了 → ループ再生開始: %s"), *myClearLoopSound->GetName());
    }
}
