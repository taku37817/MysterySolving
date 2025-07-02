// Fill out your copyright notice in the Description page of Project Settings.


#include "Setting.h"
#include "GameFramework/PlayerController.h"     // プレイヤーコントローラー関連クラス
#include "Kismet/GameplayStatics.h" // プレイヤー取得やサウンド再生などの静的ユーティリティ
#include "GetUObjectClass.h"

// Sets default values
ASetting::ASetting()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ASetting::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void ASetting::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if(bIsVisible)
	{
		APlayerController* playerController = UGameplayStatics::GetPlayerController(this, 0); // プレイヤーコントローラー取得
		if (settingWidgetClass)
		{
			UUserWidget* pauseWidget = CreateWidget<UUserWidget>(playerController, settingWidgetClass); // ウィジェットの作成
		
			if (pauseWidget && pauseWidget->IsInViewport())  // ビューに追加されている時のみ
			{

				pauseWidget->RemoveFromParent();  // 親から削除
				pauseWidget->SetVisibility(ESlateVisibility::Collapsed); // 強制的に非表示に設定
				UE_LOG(LogTemp, Display, TEXT("👍👍👍👍"));
			
			}
		}
		bIsVisible = false;
	}

}

void ASetting::OnBackButtonClicked()
{
	UE_LOG(LogTemp, Display, TEXT("call👍"));
	bIsVisible = true;

}



