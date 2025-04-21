// Fill out your copyright notice in the Description page of Project Settings.


#include "MultipleConditioDoorMover.h"
#include "Engine/World.h"

// Sets default values for this component's properties
UMultipleConditioDoorMover::UMultipleConditioDoorMover()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

    // 比較用タグを設定
    hikakuTag.Add(FName("Gargoyle_Red"));
    hikakuTag.Add(FName("Gargoyle_Blue"));
    hikakuTag.Add(FName("Gargoyle_Green"));

	// ...
}


// Called when the game starts
void UMultipleConditioDoorMover::BeginPlay()
{
	Super::BeginPlay();
	
}


// Called every frame
void UMultipleConditioDoorMover::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
    deltaTime = DeltaTime;
    CheckConditions();
}
// 条件をチェックするメソッド
void UMultipleConditioDoorMover::CheckConditions()
{
    // すべての TriggerComponent のアクターから取得したタグを格納する配列
    TArray<FName> totalTags;

    // 各 TriggerComponent アクターに対して処理を行う
    for (AActor* triggerComponentActor : triggerComponentActors)
    {
        // アクターから TriggerComponent を探す（存在する前提）
        UTriggerComponent* triggerComp = triggerComponentActor->FindComponentByClass<UTriggerComponent>();
        if (!triggerComp)
        {
            // もし TriggerComponent が見つからなければエラーログを出して終了
            UE_LOG(LogTemp, Error, TEXT("TriggerComponent is null!"));
            return;
        }

        // TriggerComponent が見つかったらログ出力
        UE_LOG(LogTemp, Warning, TEXT("TriggerComponent is valid: %s"), *triggerComp->GetName());

        // この TriggerComponent にオーバーラップしているアクターのタグを取得
        TArray<FName> actorTags = triggerComp->GetOverlappingActorTags();

        // 取得したタグを totalTags に追加（AddUnique で重複回避）
        for (const FName& tag : actorTags)
        {
            totalTags.AddUnique(tag);
        }
    }

    // 現在オーバーラップしている全タグをログに表示（デバッグ用）
    for (const FName& tag : totalTags)
    {
        UE_LOG(LogTemp, Warning, TEXT("重なっているタグ: %s"), *tag.ToString());
    }

    // すべての比較対象タグ（hikakuTag）が totalTags に含まれているかチェック
    bool allConditionsMet = true;
    for (const FName& requiredTag : hikakuTag)
    {
        if (!totalTags.Contains(requiredTag))
        {
            // 1つでも足りなければ false にして break（処理を中断）
            allConditionsMet = false;
            break;
        }
    }

    // ドアを操作するためのコンポーネントを取得
    UDoorMover* doorMoverComponent = GetOwner()->FindComponentByClass<UDoorMover>();
    if (!doorMoverComponent)
    {
        // ドアコンポーネントがなければ警告ログ出して終了
        UE_LOG(LogTemp, Warning, TEXT("Not DoorMoverComponent！"));
        return;
    }

    // すべての条件を満たしていた場合（ドアを開ける）
    if (allConditionsMet)
    {
        // 移動処理の前準備とタイマー開始
        doorMoverComponent->StorneDoorMoverFunction(deltaTime);
        doorMoverComponent->StartTimer();
        doorMoverComponent->SetShouldMove(true);  // ドアを開ける

        // ログ表示
        UE_LOG(LogTemp, Warning, TEXT("条件を満たしたのでゲートを開けました！"));
    }
    else
    {
        // 条件未達時：同様に動かすがフラグは閉じる
        doorMoverComponent->StorneDoorMoverFunction(deltaTime);
        doorMoverComponent->StartTimer();
        doorMoverComponent->SetShouldMove(false);  // ドアを閉める

        // ログ表示
        UE_LOG(LogTemp, Warning, TEXT("条件が満たされていません"));
    }
}