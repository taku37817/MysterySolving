// Fill out your copyright notice in the Description page of Project Settings.


#include "CandleAndPointLightOn.h"  // このクラスのヘッダーファイル
#include "NiagaraComponent.h"  // Niagaraエフェクトのコンポーネント用
#include "NiagaraFunctionLibrary.h" // エフェクトのスポーンなどに使用するライブラリ
#include "Components/PointLightComponent.h"  // ポイントライトのコンポーネント用


// Sets default values for this component's properties
UCandleAndPointLightOn::UCandleAndPointLightOn()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
}

// Called when the game starts
void UCandleAndPointLightOn::BeginPlay()
{
	Super::BeginPlay();
}


// Called every frame
void UCandleAndPointLightOn::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);  // 親クラスのTickComponentを呼び出す
    CandleAndPointLightOn();  // キャンドルとライトを点灯する処理を呼び出す
}
//キャンドルとポイントライト点灯
void UCandleAndPointLightOn::CandleAndPointLightOn()
{
    AActor* candleActor = GetOwner();  // このコンポーネントが属するアクター（＝キャンドル）を取得
    if(!candleActor)  // 取得できなかった場合のエラーチェック
    {
        UE_LOG(LogTemp, Warning, TEXT("❌Not candleActor!"));  // デバッグログ出力
        return;  // 処理中断
    }

    if(IsOverlappingActor(grabbedCandleActor))  // 他のアクター（掴んだキャンドル）と重なっているか確認
    {
        if(candleActor->GetName().Contains("Candle"))  // 名前に「Candle」が含まれているか確認
        {
            if (UNiagaraComponent* niagaraComp = candleActor->FindComponentByClass<UNiagaraComponent>())  // Niagaraコンポーネントを取得
            {
                if(!niagaraComp)  // 取得失敗したらログ出して終了
                {
                    UE_LOG(LogTemp, Warning, TEXT("❌Not niagaraComp!"));
                    return;
                }
                niagaraComp->SetVisibility(true);  // Niagaraエフェクトを表示（点火エフェクトなど）
            }
        }

        if(pointLightActor)  // ポイントライトが設定されている場合
        {
            if (UPointLightComponent* lightComp = pointLightActor->FindComponentByClass<UPointLightComponent>())  // ライトコンポーネントを取得
            {
                if(!lightComp)  // ライト取得できなかったらログ出して終了
                {
                    UE_LOG(LogTemp, Warning, TEXT("❌Not lightComp!"));
                    return;
                }
                lightComp->SetVisibility(true);  // ポイントライトを表示（光らせる）
            }

            UE_LOG(LogTemp, Display, TEXT("🕯️ Candle lit!"));  // 成功ログ（点火完了）
        }
    }
}