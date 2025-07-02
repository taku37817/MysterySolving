// 複数の条件を満たすことでドアを開閉するコンポーネントの定義
#pragma once

#include "CoreMinimal.h"  // 基本的な型やマクロを含むUnrealのコアライブラリ
#include "Components/ActorComponent.h"  // ActorComponentの機能を使用するためのヘッダー
#include "MultipleConditioDoorMover.generated.h"  // UnrealHeaderToolによるコード生成を補助するマクロを含む

// 前方宣言：必要なクラスの参照
class UTriggerComponent;  // トリガーの状態を取得するためのクラス
class UDoorMover;  // ドアの開閉制御を行うためのクラス

/**
 * UMultipleConditioDoorMover
 * 複数のスタンド（トリガー）を監視し、それぞれに対応するタグ（像）が正しく揃った場合のみ、
 * ドアを開閉する処理を行うアクターコンポーネント。
 * 各ドアごとにこのクラスを個別にアタッチし、監視対象と必要タグを設定する。
 */
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class MYSTERYSOLVING_API UMultipleConditioDoorMover : public UActorComponent
{
    GENERATED_BODY()

public: 
    /** コンストラクタ：Tickを有効化 */
    UMultipleConditioDoorMover();

protected:
    /** ゲーム開始時に呼ばれる初期化処理 */
    virtual void BeginPlay() override;

public: 
    /** 毎フレーム呼ばれる処理。条件チェックを行う */
    virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	bool isAllConditionsMet = false;

private:
    /** 条件を評価し、ドアを制御するメソッド */
    void CheckConditions();

    /** Tickで受け取るDeltaTimeの保持用 */
    float deltaTime = 0.0f;

    /** 前フレームで条件が成立していたか（状態変化判定用） */
    bool wasDoorOpen = false;

    /** このドアが監視するスタンド（TriggerComponentを持つアクター）の一覧 */
    UPROPERTY(EditAnywhere)
    TArray<AActor*> myTriggerActors;

    /** このドアを開けるために必要な像のタグ名一覧（例：Gargoyle_Red など） */
    UPROPERTY(EditAnywhere)
    TArray<FName> requiredTags;
};
