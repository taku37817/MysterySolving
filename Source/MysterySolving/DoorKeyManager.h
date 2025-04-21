// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/BoxComponent.h"
#include "Blueprint/UserWidget.h"  // UUserWidgetのインクルード
#include "Components/Image.h" // UImageを使うためのインクルード
#include "DoorKeyManager.generated.h"

// FKeyWidgetData構造体を宣言（鍵の種類とアイコン画像を持つ）
USTRUCT(BlueprintType)  // Blueprint上でも使える構造体として定義
struct FKeyWidgetData
{
    GENERATED_BODY()  // 必要なUnreal Engineマクロ（Reflectionシステム用）

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FName keyTag;  // タグ名

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TSubclassOf<UUserWidget> keyWidgetClass;  // 対応するWBPクラス
};

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class MYSTERYSOLVING_API UDoorKeyManager : public UBoxComponent
{
    GENERATED_BODY()
public:
    // コンストラクタ: デフォルト値の設定
    UDoorKeyManager();

protected:
    // ゲーム開始時に呼ばれる
    virtual void BeginPlay() override;

   // 鍵のWBPクラスの配列
   UPROPERTY(EditAnywhere, Category="Widgets")
   TArray<FKeyWidgetData> keyWidgetClasses;  // WBP名とタグ名の対応を管理する配列

   UPROPERTY()
   UUserWidget* currentKeyWidget;  // 現在表示中のWBP

public:
    // 毎フレーム呼ばれる処理
    virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

    //


    // 鍵に応じたアイコンを表示するメソッド
    // KeyTagに基づいて、適切な鍵のアイコンをウィジェットで表示する
    void ShowKeyWidget(FName KeyTag);
    
    // 扉を開けようとする処理
    void TryOpenDoor();
    
    // 鍵を取得する処理
    void CollectKey(FName KeyTag);
    
    // 指定の鍵で扉を開けられるか判定
    bool CanUseKey(FName DoorTag) const;

    // 扉の近くにいるか (Blueprintで利用)
    UPROPERTY(BlueprintReadWrite, Category="_Door")
    bool bIsNearDoor = false;
    
    // メッセージ表示フラグ (Eキーを押せるかどうか)
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="_Door")
    bool bShowMessage = false;
    
    // 扉を開けられるかどうか
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="_Door")
    bool bCanOpenDoor = false;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="_Door")
    bool isOpenDoor = false;

    // // 左のドアを開ける処理 (BlueprintCallable)
    // UFUNCTION(BlueprintCallable)
    // void LeftDoorOpen();

private:
    // 取得した鍵のタグリスト (順番保持)
    TArray<FName> collectedKeyTags;
    
    // 指定タグがついているアクターを取得
    TArray<AActor*> FindActorByName(UWorld* World, const FName& ActorTag);
    
    UPROPERTY(EditAnywhere)
    TArray<FName> doorTags = { "GoldDoor","SilverDoor","BrowneDoor" }; // 指定タグがついているアクターを取得するための変数
    
    UWorld* world; // GetWorld() を使用するための変数
    
    // 指定タグがついているドアアクターのマップ (タグごとのリスト)
    TMap<FName, TArray<AActor*>> taggedDoorActors;
    
    // 指定タグがついたドアアクターを登録する処理
    void RegisterDoors();
};
