#pragma once 

#include "CoreMinimal.h" 
#include "Blueprint/UserWidget.h" // UUserWidget を使うためのヘッダー
#include "TitleOperation.generated.h" 

UCLASS() // Unreal Engine 用のクラスを定義するマクロ
class MYSTERYSOLVING_API UTitleOperation : public UUserWidget // UUserWidget を継承したタイトル用 UI クラス
{
    GENERATED_BODY() // UCLASS の中に書くことで Unreal の内部処理と連携するためのマクロ

protected:
    virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override; // 毎フレーム呼ばれる処理（Tick相当）

public:
    UFUNCTION(BlueprintCallable) // ブループリントから呼び出せる関数にする
    void OnGameStartButtonClicked(); // 「ゲームスタート」ボタンが押された時の処理

    UFUNCTION(BlueprintCallable) // ブループリントから呼び出せる関数にする
    void OnSettingButtonClicked(); // 「設定」ボタンが押された時の処理

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI", meta = (AllowPrivateAccess = "true")) // ウィジェットを指定するためのプロパティ
    TSubclassOf<UUserWidget> TitleWidgetClass; // 表示したいウィジェットクラス（別の UI を出したいときなど）
};
