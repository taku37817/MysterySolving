// Fill out your copyright notice in the Description page of Project Settings.


#include "FinalMysterySolvingRoomClass.h"
#include "MultipleConditioDoorMover.h"
#include "Particles/ParticleSystemComponent.h"  // ナビゲーションパーティクルのインクルード
#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"           // エフェクトをスポーンしたりしたい時に必要
#include "Components/PointLightComponent.h"
#include "Engine/World.h"
#include "EngineUtils.h"
#include "GameFramework/Actor.h"
#include "Components/StaticMeshComponent.h"
#include "DoorMover.h"
#include "Grabber.h"

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
	Super::BeginPlay();
    bLightingManualOn = true;
	World = GetWorld();
	if (!World)
    {
        UE_LOG(LogTemp, Error, TEXT("UTriggerComponent: GetWorld() returned nullptr!"));
    }
}


// Called every frame
void UFinalMysterySolvingRoomClass::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
    UDoorMover* doorMoverComponent = doorActor->FindComponentByClass<UDoorMover>();
    doorMoverComponent->StorneDoorMoverFunction(DeltaTime);
	deltaTime = DeltaTime;
    StateOfTheCandle();
	// OverlapJudgmentFunction(); //オーバーラップしてるか
	DoYouHaveBronzeStatueOrPlaced(); //銅像を持ってるかまたは置かれているか
	CandleDoYouHaveFunction(); //Candleを持ってるか
}
//銅像持ってた場合ナビゲーター(ロウソクの火)を消す
void UFinalMysterySolvingRoomClass::CandlEeraseFunction(bool isCandleVisibility)
{
	for (AActor* child : candleActors)
    {
        if (!child) continue;

        UE_LOG(LogTemp, Warning, TEXT("child Comp: %s"), *child->GetName());
        // ナビゲーションパーティクルの処理
        if (child->GetName().Contains("Candle"))
        {
            if (UNiagaraComponent* niagaraComp = child->FindComponentByClass<UNiagaraComponent>())
            {
                UE_LOG(LogTemp, Warning, TEXT("Niagara Comp: %s"), *niagaraComp->GetName());

                // パーティクルを制御
                if (isCandleVisibility)
                {
                    niagaraComp->SetVisibility(true);  // 描画を再開
                    UE_LOG(LogTemp, Display, TEXT("🔥 Niagara Activated and Visible"));
                }
                else
                {
                    niagaraComp->SetVisibility(false);  // 描画を非表示
                    UE_LOG(LogTemp, Display, TEXT("🧯 Niagara Deactivated and Hidden"));
                }
            }
        }
        // ポイントライトの処理
        for(AActor* pointLightActor : pointLightActors)
        {
            if (pointLightActor->GetName().Contains("PointLight"))
            {
                if (UPointLightComponent* lightComp = pointLightActor->FindComponentByClass<UPointLightComponent>())
                {
                    lightComp->SetVisibility(isCandleVisibility);
                }
            }
        }        
    }
}
//キャンドルが点いているか
void UFinalMysterySolvingRoomClass::StateOfTheCandle()
{
    bool bAllHidden = true, bAllVisible = true;
    
	for (AActor* child : candleActors)
    {
        if (!child) continue;

        bool bParticleVisible = false;
        bool bLightVisible = false;
        UE_LOG(LogTemp, Warning, TEXT("child Comp: %s"), *child->GetName());
        // ナビゲーションパーティクルの処理
        if (child->GetName().Contains("Candle"))
        {
            if (UNiagaraComponent* niagaraComp = child->FindComponentByClass<UNiagaraComponent>())
            {
                bParticleVisible = niagaraComp->IsVisible();
            }
        }

        // ポイントライトの処理
        for(AActor* pointLightActor : pointLightActors)
        {
            if (pointLightActor->GetName().Contains("PointLight"))
            {
                if (UPointLightComponent* lightComp = pointLightActor->FindComponentByClass<UPointLightComponent>())
                {
                    bLightVisible = lightComp->IsVisible();
                }
            }
        }

        // **炎と光が両方ついていたら true、どちらかでも消えていたら false**
        bool bCandleVisible = bParticleVisible && bLightVisible;
        bAllHidden &= !bCandleVisible;  // 1つでもついていたら bAllHidden = false
        bAllVisible &= bCandleVisible;  // 1つでも消えていたら bAllVisible = false
        UE_LOG(LogTemp, Warning, TEXT("🔥 Particle Visible: %s"), bParticleVisible ? TEXT("true") : TEXT("false"));
        UE_LOG(LogTemp, Warning, TEXT("💡 Light Visible: %s"), bLightVisible ? TEXT("true") : TEXT("false"));
        UE_LOG(LogTemp, Warning, TEXT("🕯️ bCandleVisible: %s"), bCandleVisible ? TEXT("true") : TEXT("false"));
        UE_LOG(LogTemp, Warning, TEXT("🔒 isCandleGrabbed: %s"), isCandleGrabbed ? TEXT("true") : TEXT("false"));
        
    }
    //すべて消えていたら isCandleGrabbed = true、すべてついていたら isCandleGrabbed = false にな
    isCandleGrabbed = bAllHidden ? true : bAllVisible ? false : isCandleGrabbed;
    UE_LOG(LogTemp, Display, TEXT("🔥 isCandleGrabbed: %s"), isCandleGrabbed ? TEXT("true") : TEXT("false"));

}
//ドアを動かせるか
void UFinalMysterySolvingRoomClass::DoorMoveJudgeFunction(bool isDoorMover)
{
	if (!doorActor)
	{
		UE_LOG(LogTemp, Warning, TEXT("❌ Not doorActor !"));
		return;
	}

	UDoorMover* doorMoverComponent = doorActor->FindComponentByClass<UDoorMover>();
	if (!doorMoverComponent)
	{
		UE_LOG(LogTemp, Warning, TEXT("❌ Not DoorMoverComponent !"));
		return;
	}

	// SetShouldMove を呼ぶ前に準備処理とタイマー開始も行う
	doorMoverComponent->StorneDoorMoverFunction(deltaTime);  // ← これがないと動かない
	doorMoverComponent->StartTimer();
	doorMoverComponent->SetShouldMove(isDoorMover); // ← フラグセットだけではダメ、実行処理も必要

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