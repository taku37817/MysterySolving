// Fill out your copyright notice in the Description page of Project Settings.


#include "StoneSpecialDoorMover.h"
#include "TriggerComponent.h"

UStoneSpecialDoorMover::UStoneSpecialDoorMover()
{
    // Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
}

void UStoneSpecialDoorMover::BeginPlay()
{
	Super::BeginPlay();
}

void UStoneSpecialDoorMover::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
    SpecialDoorMover();
    for (AActor* doorActor : doorActors)
    {
        UDoorMover* doorComponent = doorActor->FindComponentByClass<UDoorMover>();
        if (doorComponent)
        {
            doorComponent->StorneDoorMoverFunction(DeltaTime);
        }
    }
    
}

void UStoneSpecialDoorMover::SpecialDoorMover()
{
    TArray<AActor*> actors; 
    GetOverlappingActors(actors); 

    AActor* standActor = GetOwner();
    AActor* acceptableActor = GetAcceptableActor();
    bool isGargoyleOnSwitch = false; // ガーゴイルがガーゴイル台に乗っているか
    bool isStandOverlapping = (acceptableActor != nullptr); // ガーゴイル台がトリガーにオーバーラップしているか

    UTriggerComponent* triggerComponent = nullptr;

    // スタンドにオーバーラップしているアクターの中から UTriggerComponent を取得
    for (AActor* actor : actors)
    {
        triggerComponent = actor->FindComponentByClass<UTriggerComponent>();
        if (triggerComponent)
        {
            isGargoyleOnSwitch = triggerComponent->gargoyleOvarlapJudge;
            break; // 1つでも見つかればOK
        }
    }

    // 開閉判定
    bool shouldOpenDoor = isGargoyleOnSwitch && isStandOverlapping;

    if (shouldOpenDoor != isStandSwitchJudge) // 状態が変わったときのみ処理
    {
        UE_LOG(LogTemp, Warning, TEXT("Door state changed: %s"), shouldOpenDoor ? TEXT("Open") : TEXT("Close"));

        for (AActor* doorActor : doorActors)
        {
            UDoorMover* doorComponent = doorActor->FindComponentByClass<UDoorMover>();
            if (doorComponent)
            {
                doorComponent->StartTimer();
                doorComponent->SetShouldMove(shouldOpenDoor); // true なら開く、false なら閉じる
            }
        }

        isStandSwitchJudge = shouldOpenDoor; // 状態更新
    }
    
}

AActor* UStoneSpecialDoorMover::GetAcceptableActor()
{
    TArray<AActor*> actors; //オーバーラップしているアクター取得のための配列
    GetOverlappingActors(actors); //オーバーラップしたアクターを取得
    for(AActor* actor : actors)  //アクターごとにループ処理
    {
        bool hasStandAcceptableTag = actor->ActorHasTag(standTag); //StandRedタグを持っているか判定
        bool isGrabbed = actor->ActorHasTag("Grabbed"); //掴まれているか判定
        if(hasStandAcceptableTag &&!isGrabbed) //許容タグがあり、掴まれていない場合
        {
            return actor; //該当アクターを返す
        }
    }
    return nullptr; //該当アクターがない場合はnullptrを返す
}