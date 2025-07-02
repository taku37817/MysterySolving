// Fill out your copyright notice in the Description page of Project Settings.


#include "FinalMysterySolvingRoomClass.h"  // このクラスのヘッダー
#include "MultipleConditioDoorMover.h"  // 複数条件ドア制御用のカスタムコンポーネント
#include "Particles/ParticleSystemComponent.h"  // パーティクルシステム用（旧Cascade）
#include "NiagaraComponent.h"  // Niagaraエフェクトコンポーネント
#include "NiagaraFunctionLibrary.h"  // Niagaraエフェクトのスポーンなどに使用
#include "Components/PointLightComponent.h"  // ポイントライトのコンポーネント取得用
#include "Engine/World.h"  // ワールド情報取得
#include "EngineUtils.h"  // アクターをワールド内で検索するためのユーティリティ
#include "GameFramework/Actor.h"  // AActorクラスの基本機能
#include "Components/StaticMeshComponent.h"  // StaticMeshコンポーネント操作用
#include "DoorMover.h"  // ドア移動用の自作コンポーネント
#include "Grabber.h"  // 掴み処理を行う自作コンポーネント


// Sets default values for this component's properties
UFinalMysterySolvingRoomClass::UFinalMysterySolvingRoomClass()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
}


// Called when the game starts
void UFinalMysterySolvingRoomClass::BeginPlay()
{
	Super::BeginPlay();  // 親クラスのBeginPlayを呼び出す
    bLightingManualOn = true;  // 初期状態ではキャンドル点灯許可をONにしておく
	World = GetWorld();  // ワールド情報を取得
	if (!World)  // ワールド取得失敗時のログ出力
    {
        UE_LOG(LogTemp, Error, TEXT("UTriggerComponent: GetWorld() returned nullptr!"));
    }
}


// Called every frame
void UFinalMysterySolvingRoomClass::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);  // 親クラスのTickComponentを呼ぶ
    UDoorMover* doorMoverComponent = doorActor->FindComponentByClass<UDoorMover>();  // ドアアクターからDoorMoverコンポーネント取得
    doorMoverComponent->StorneDoorMoverFunction(DeltaTime);  // ドアの移動処理を毎フレーム呼び出す
	deltaTime = DeltaTime;  // DeltaTimeをメンバ変数に保存
    StateOfTheCandle();  // キャンドルの状態チェック処理を呼ぶ
	// OverlapJudgmentFunction(); //オーバーラップしてるか（※現在はコメントアウト）
	DoYouHaveBronzeStatueOrPlaced(); // 銅像を持っているかまたは置いているかの処理
	CandleDoYouHaveFunction(); // Candleをプレイヤーが持っているかの処理
}

//銅像持ってた場合ナビゲーター(ロウソクの火)を消す
void UFinalMysterySolvingRoomClass::CandlEeraseFunction(bool isCandleVisibility)
{
	for (AActor* child : candleActors)  // candleActors に含まれる全てのアクターに対してループ
    {
        if (!child) continue;  // nullptrならスキップ

        UE_LOG(LogTemp, Warning, TEXT("child Comp: %s"), *child->GetName());  // ログ出力（確認用）

        // ナビゲーションパーティクルの処理（名前に "Candle" を含むかチェック）
        if (child->GetName().Contains("Candle"))
        {
            if (UNiagaraComponent* niagaraComp = child->FindComponentByClass<UNiagaraComponent>())  // Niagaraコンポーネント取得
            {
                UE_LOG(LogTemp, Warning, TEXT("Niagara Comp: %s"), *niagaraComp->GetName());  // コンポ名ログ

                // パーティクルの表示・非表示切り替え
                if (isCandleVisibility)
                {
                    niagaraComp->SetVisibility(true);  // 表示する（点火）
                    UE_LOG(LogTemp, Display, TEXT("🔥 Niagara Activated and Visible"));
                }
                else
                {
                    niagaraComp->SetVisibility(false);  // 非表示にする（消火）
                    UE_LOG(LogTemp, Display, TEXT("🧯 Niagara Deactivated and Hidden"));
                }
            }
        }

        // ポイントライトの処理（複数あっても対応）
        for(AActor* pointLightActor : pointLightActors)
        {
            if (pointLightActor->GetName().Contains("PointLight"))  // 名前に "PointLight" を含むかチェック
            {
                if (UPointLightComponent* lightComp = pointLightActor->FindComponentByClass<UPointLightComponent>())  // ライト取得
                {
                    lightComp->SetVisibility(isCandleVisibility);  // 表示／非表示切り替え
                }
            }
        }        
    }
}

//キャンドルが点いているか
void UFinalMysterySolvingRoomClass::StateOfTheCandle()
{
    bool bAllHidden = true, bAllVisible = true;  // 初期状態：全部消えてる＆全部点いてる 両方 true（途中で変更される）

	for (AActor* child : candleActors)  // 各キャンドルアクターについて処理
    {
        if (!child) continue;

        bool bParticleVisible = false;  // パーティクルが表示されているか
        bool bLightVisible = false;  // ライトが表示されているか

        UE_LOG(LogTemp, Warning, TEXT("child Comp: %s"), *child->GetName());  // 子アクターのログ

        // パーティクル表示確認
        if (child->GetName().Contains("Candle"))
        {
            if (UNiagaraComponent* niagaraComp = child->FindComponentByClass<UNiagaraComponent>())
            {
                bParticleVisible = niagaraComp->IsVisible();  // 表示されているか
            }
        }

        // ライト表示確認
        for(AActor* pointLightActor : pointLightActors)
        {
            if (pointLightActor->GetName().Contains("PointLight"))
            {
                if (UPointLightComponent* lightComp = pointLightActor->FindComponentByClass<UPointLightComponent>())
                {
                    bLightVisible = lightComp->IsVisible();  // 表示されているか
                }
            }
        }

        // 両方が表示されていれば true（炎と光）
        bool bCandleVisible = bParticleVisible && bLightVisible;

        bAllHidden &= !bCandleVisible;  // 一つでも点いていたら false になる
        bAllVisible &= bCandleVisible;  // 一つでも消えていたら false になる

        // 状態ログ出力
        UE_LOG(LogTemp, Warning, TEXT("🔥 Particle Visible: %s"), bParticleVisible ? TEXT("true") : TEXT("false"));
        UE_LOG(LogTemp, Warning, TEXT("💡 Light Visible: %s"), bLightVisible ? TEXT("true") : TEXT("false"));
        UE_LOG(LogTemp, Warning, TEXT("🕯️ bCandleVisible: %s"), bCandleVisible ? TEXT("true") : TEXT("false"));
        UE_LOG(LogTemp, Warning, TEXT("🔒 isCandleGrabbed: %s"), isCandleGrabbed ? TEXT("true") : TEXT("false"));
    }

    // 状態更新：全消灯→true、全点灯→false、それ以外は変化なし
    isCandleGrabbed = bAllHidden ? true : bAllVisible ? false : isCandleGrabbed;

    UE_LOG(LogTemp, Display, TEXT("🔥 isCandleGrabbed: %s"), isCandleGrabbed ? TEXT("true") : TEXT("false"));
}

//ドアを動かせるか
void UFinalMysterySolvingRoomClass::DoorMoveJudgeFunction(bool isDoorMover)
{
	if (!doorActor)  // ドアが設定されていない場合
	{
		UE_LOG(LogTemp, Warning, TEXT("❌ Not doorActor !"));
		return;
	}

	UDoorMover* doorMoverComponent = doorActor->FindComponentByClass<UDoorMover>();  // DoorMoverコンポーネント取得
	if (!doorMoverComponent)
	{
		UE_LOG(LogTemp, Warning, TEXT("❌ Not DoorMoverComponent !"));
		return;
	}

	// ドア移動処理：フラグ＋即時移動＋音声付きタイマー処理
	doorMoverComponent->StorneDoorMoverFunction(deltaTime);  // 実際に移動処理を行う
	doorMoverComponent->StartTimer();  // タイマー開始（音声など）
	doorMoverComponent->SetShouldMove(isDoorMover); // フラグを設定（開く or 閉じる）

	UE_LOG(LogTemp, Warning, TEXT("✅ ドア状態変更実行: %s"), isDoorMover ? TEXT("Open") : TEXT("Close"));
}



//銅像を持ってるかまたは置かれているか
void UFinalMysterySolvingRoomClass::DoYouHaveBronzeStatueOrPlaced()
{
    AActor* playerActor = FindActorByName(GetWorld(), TEXT("BP_Player")); //WorldにBP_Playerが存在したらplayerActorに代入
	UGrabber* grabberComponent = playerActor->FindComponentByClass<UGrabber>();
	if(!grabberComponent)
	{
		UE_LOG(LogTemp, Warning, TEXT("❌Not grabberComponent!"));
		return;	
	}
    if(!finalDoorActor) return;
    UMultipleConditioDoorMover* multipleConditioDoorMover = finalDoorActor->FindComponentByClass<UMultipleConditioDoorMover>();
    if(multipleConditioDoorMover && multipleConditioDoorMover->isAllConditionsMet)
    {
        if (grabberComponent->isBronzeStatueGrabbed)
        {
            if (bLightingManualOn) // 点火前の状態のみ処理する
            {
                CandlEeraseFunction(false); // 火を消す
                DoorMoveJudgeFunction(false); // ドアを閉める
                bLightingManualOn = false;
            }
        }

        else
        {
            // 銅像アクターがオーバーラップしているか確認
            if (bronzeStatueActor && IsOverlappingActor(bronzeStatueActor))
            {
                if(!bLightingManualOn)
                {
                    // 像がこのスタンドに乗っているなら開ける
                    DoorMoveJudgeFunction(true); // ドアを開く
                    CandlEeraseFunction(true);   // キャンドル再点灯
                    bLightingManualOn = true;    // 手動点火を無効化
                    UE_LOG(LogTemp, Display, TEXT("✅ BronzeStatue is placed correctly and overlapping."));
                }
            }
            else
            {
                if (bLightingManualOn) // 点火前の状態のみ処理する
                {
                    CandlEeraseFunction(false); // 火を消す
                    DoorMoveJudgeFunction(false); // ドア閉じる
                    UE_LOG(LogTemp, Display, TEXT("❌ BronzeStatue is not overlapping."));
                    bLightingManualOn = false; 
                }
            }
            //CandlEeraseFunction(true);
            UE_LOG(LogTemp, Warning, TEXT("❌Not 銅像 Grabbed!"));
        }
        if(isCandleGrabbed) //すべて消えていたら isCandleGrabbed = true
        {
            DoorMoveJudgeFunction(false); //すべてついていたら、ドアが閉じる
            if(bIsCandleGrabbedMessageOnlyOnce)
            {
                if(grabberComponent)
                    grabberComponent->bIsCandleGrabbedMessage = true; //キャンドル持てるメッセージ表示
                bIsCandleGrabbedMessageOnlyOnce = false; //持てるかキャンドルメッセージのbool判定を適用を一回のみにするための判定
            }
        }
        else //すべてついていたら isCandleGrabbed = false
        {
            DoorMoveJudgeFunction(true); //すべてついていたら、ドアが開く
            if(!bIsCandleGrabbedMessageOnlyOnce)
            {
                if(grabberComponent)
                    grabberComponent->bIsCandleGrabbedMessage = false; //キャンドル持てるメッセージ非表示
                bIsCandleGrabbedMessageOnlyOnce = true; //持てるかキャンドルメッセージのbool判定を適用を一回のみにするための判定
            }
        }
    }
    else if(multipleConditioDoorMover && !multipleConditioDoorMover->isAllConditionsMet)
    {
         DoorMoveJudgeFunction(false); // ドアが閉まる
    }
}

// 指定した名前のアクターをワールド内で検索する関数
AActor* UFinalMysterySolvingRoomClass::FindActorByName(UWorld* world, const FString& actorName)
{
    if (world)  // ワールドが有効かどうかを確認
    {
        // TActorIteratorを使って、ワールド内のすべてのアクターをイテレートする
        for (TActorIterator<AActor> actorIterator(world); actorIterator; ++actorIterator)
        {
            // 現在のアクターの名前が一致するかどうかを確認
            if (actorIterator->GetName().Contains(actorName))
            {
                return *actorIterator;  // 一致するアクターを見つけた場合、アクターを返す
            }
        }
    }
    return nullptr;  // 一致するアクターが見つからなければnullptrを返す
}

//オーバーラップ中のアクターを取得する関数
AActor* UFinalMysterySolvingRoomClass::GetAcceptableActor()
{
    TArray<AActor*> actors; //オーバーラップしているアクター取得のための配列
    GetOverlappingActors(actors); //オーバーラップしたアクターを取得
    for(AActor* actor : actors)  //アクターごとにループ処理
    {
        bool hasAcceptableTag = actor->ActorHasTag(acceptableActorTag); //許容タグを持っているか判定(銅像)
        bool isGrabbed = actor->ActorHasTag("Grabbed"); //掴まれているか判定
        if(hasAcceptableTag && !isGrabbed) //許容タグがあり、掴まれていない場合
        {
            return actor; //該当アクターを返す
        }
    }
    return nullptr; //該当アクターがない場合はnullptrを返す
}

//Candleを持ってるか
void UFinalMysterySolvingRoomClass::CandleDoYouHaveFunction() 
{
    ECollisionChannel grabberChannel = ECC_GameTraceChannel2;
    UStaticMeshComponent* meshComponent = grabbedCandleActor->FindComponentByClass<UStaticMeshComponent>();
    meshComponent->SetCollisionResponseToChannel(grabberChannel,isCandleGrabbed ? ECR_Block : ECR_Ignore);
}
// //オーバーラップしてるか
// void UFinalMysterySolvingRoomClass::OverlapJudgmentFunction()
// {
// 	AActor* playerActor = FindActorByName(GetWorld(), TEXT("BP_Player")); //WorldにBP_Playerが存在したらplayerActorに代入
// 	if(!playerActor) UE_LOG(LogTemp, Warning, TEXT("❌　Not playerActor!")) return;
//     UGrabber* grabberComponent = playerActor->FindComponentByClass<UGrabber>();
//     if(IsOverlappingActor(grabbedCandleActor))
//     {
//         if(!grabberComponent) UE_LOG(LogTemp, Warning, TEXT("❌Not grabberComponent!")) return;
//         grabberComponent->SetGargoyleAndStandMessageDisplayJudge(false,true);
//     }
// }