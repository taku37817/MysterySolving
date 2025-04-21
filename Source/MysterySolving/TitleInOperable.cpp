// Fill out your copyright notice in the Description page of Project Settings.


#include "TitleInOperable.h"
#include "Kismet/GameplayStatics.h" // プレイヤー取得・レベル遷移・音再生などの便利関数を提供するヘッダー
#include "GameFramework/PlayerController.h" // 入力制御に必要
#include "GameFramework/Character.h" // キャラクタークラス
#include "GameFramework/CharacterMovementComponent.h" // 移動停止処理に必要


// Sets default values
ATitleInOperable::ATitleInOperable()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ATitleInOperable::BeginPlay()
{
	Super::BeginPlay();
	NotOperation();
}

// Called every frame
void ATitleInOperable::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// キャラクターの操作を無効にし、UIのみに集中させる処理
void ATitleInOperable::NotOperation()
{
	APlayerController* playerController = UGameplayStatics::GetPlayerController(this, 0); // プレイヤーコントローラ取得
    if (playerController && titlWidgetClass) // 存在チェック
    {
		titleWidget = CreateWidget<UUserWidget>(playerController,titlWidgetClass);
		if(titleWidget)
		{
			titleWidget->AddToViewport(); // ← 必須！
			FInputModeUIOnly inputMode; // UI専用の入力モードを作成
			inputMode.SetWidgetToFocus(titleWidget->TakeWidget()); // 現在のウィジェットにフォーカス設定
			inputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock); // マウスロックなし
			playerController->SetInputMode(inputMode); // UI専用入力モードに切り替え
			playerController->bShowMouseCursor = true; // マウスカーソルを表示

			playerController->DisableInput(playerController); // プレイヤー入力を無効化
		
        	APawn* playerPawn = playerController->GetPawn(); // ポーン取得
		
			if (playerPawn) // ポーンが有効なら
			{
				playerPawn->DisableInput(playerController); // ポーンの入力も無効化

				ACharacter* character = Cast<ACharacter>(playerPawn); // キャラクターにキャスト
				if (character && character->GetCharacterMovement()) // キャラと移動コンポーネントが有効かチェック
				{
					character->GetCharacterMovement()->StopMovementImmediately(); // 動きの停止
					character->GetCharacterMovement()->DisableMovement(); // 移動自体を無効に
				}
        	}
		}
    }
}

