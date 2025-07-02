#include "MultipleConditioDoorMover.h"
#include "TriggerComponent.h"
#include "DoorMover.h"

// コンストラクタ：Tickを有効にすることで毎フレーム処理が可能に
UMultipleConditioDoorMover::UMultipleConditioDoorMover()
{
    PrimaryComponentTick.bCanEverTick = true;
}

// BeginPlay：ゲーム開始時またはスポーン時に呼び出される初期化処理（今回は未使用）
void UMultipleConditioDoorMover::BeginPlay()
{
    Super::BeginPlay();
}

// TickComponent：毎フレーム呼び出され、条件チェックを実行する
void UMultipleConditioDoorMover::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
    deltaTime = DeltaTime;

    CheckConditions(); // スタンドとタグの条件を確認し、ドア制御を行う
}

// CheckConditions：ドアに対応するスタンドの状態を調べ、必要なタグが揃っているかをチェック
void UMultipleConditioDoorMover::CheckConditions()
{
    TArray<FName> currentTags;  // このドア専用のスタンドから取得したタグを格納

    // 登録されたTriggerActor（スタンド）を走査
    for (AActor* actor : myTriggerActors)
    {
        UTriggerComponent* trigger = actor->FindComponentByClass<UTriggerComponent>();
        if (!trigger) continue; // TriggerComponentが存在しない場合はスキップ

        // スタンドに乗っているアクターのタグを取得
        TArray<FName> tags = trigger->GetOverlappingActorTags();

        // 重複を避けつつタグを追加
        for (const FName& tag : tags)
        {
            currentTags.AddUnique(tag);
        }
    }

    // 指定されたすべてのタグが currentTags に含まれているかを確認
    bool allTagsMatched = true;
    for (const FName& tag : requiredTags)
    {
        if (!currentTags.Contains(tag))
        {
            allTagsMatched = false;
            break; // 一つでも一致しなければ失敗とみなす
        }
    }

    // ドアのコンポーネントを取得
    UDoorMover* doorMover = GetOwner()->FindComponentByClass<UDoorMover>();
    if (!doorMover) return; // ドアが見つからなければ処理中断
    doorMover->StorneDoorMoverFunction(deltaTime); // 移動処理の初期化

    // 状態が変化した場合のみ、ドアの開閉を実行
    if (allTagsMatched && !wasDoorOpen)
    {
        isAllConditionsMet = true;
        doorMover->StartTimer(); // タイマー開始
        doorMover->SetShouldMove(true); // ドアを開ける
        UE_LOG(LogTemp, Warning, TEXT("✅ 条件が揃ったのでドアを開けました"));
        wasDoorOpen = true;
    }
    else if (!allTagsMatched && wasDoorOpen)
    {
        isAllConditionsMet = false;
        doorMover->StartTimer(); // タイマー開始
        doorMover->SetShouldMove(false); // ドアを閉じる
        UE_LOG(LogTemp, Warning, TEXT("❌ 条件が崩れたのでドアを閉じました"));
        wasDoorOpen = false;
    }
}
