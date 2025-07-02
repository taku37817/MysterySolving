#include "DoorKeyManager.h"
#include "Engine/World.h"
#include "EngineUtils.h" // TActorIteratorを使用するために必要
#include "Kismet/GameplayStatics.h"
#include "LeftOrRightOrBothDoorMovement.h"

// コンストラクタ: デフォルト値を設定
UDoorKeyManager::UDoorKeyManager()
{
    // Tickを有効化（毎フレーム実行）
    PrimaryComponentTick.bCanEverTick = true;
    // ワールドを取得
    world = GetWorld();
}

// ゲーム開始時に呼ばれる
void UDoorKeyManager::BeginPlay()
{
    Super::BeginPlay();

    // 各タグごとに対応するドアを取得し、登録
    for (const FName& tag : doorTags)
    {
        TArray<AActor*> foundDoors;
        UGameplayStatics::GetAllActorsWithTag(GetWorld(), tag, foundDoors);

        if (foundDoors.Num() > 0)
        {
            // タグごとのドアリストに追加
            taggedDoorActors.Add(tag, foundDoors);
            UE_LOG(LogTemp, Warning, TEXT("%s のドアを %d 個登録"), *tag.ToString(), foundDoors.Num());
        }
    }
}

// 毎フレーム実行
void UDoorKeyManager::TickComponent(float deltaTime, ELevelTick tickType, FActorComponentTickFunction* thisTickFunction)
{
    Super::TickComponent(deltaTime, tickType, thisTickFunction);
    // 扉を開ける処理を実行
    TryOpenDoor();
    UE_LOG(LogTemp, Display, TEXT("isOpenDoor:%s"), isOpenDoor ? TEXT("True") : TEXT("False"));
}

// カギを取得する処理
void UDoorKeyManager::CollectKey(FName keyTag)
{
    // すでに取得済みでなければ追加
    if (!collectedKeyTags.Contains(keyTag))
    {
        collectedKeyTags.Add(keyTag);
        ShowKeyWidget(keyTag);
        UE_LOG(LogTemp, Warning, TEXT("カギ %s をリストに追加！"), *keyTag.ToString());
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("カギ %s はすでにリストにある！"), *keyTag.ToString());
    }
}

// 鍵アイコンを表示するメソッド
void UDoorKeyManager::ShowKeyWidget(FName KeyTag)
{ 
	 // 対応するWBPを探す
	 for (const FKeyWidgetData& widgetData : keyWidgetClasses)
	 {
		 if (widgetData.keyTag == KeyTag)
		 {
			 // 対応するWBPを作成して表示
			 if (widgetData.keyWidgetClass)
			 {
				 // 新しいWBPを作成
				 currentKeyWidget = CreateWidget<UUserWidget>(GetWorld(), widgetData.keyWidgetClass);
 
				 if (currentKeyWidget)
				 {
					 // WBPを画面に表示
					 currentKeyWidget->AddToViewport();
					 currentKeyWidget->SetVisibility(ESlateVisibility::Visible);
				 }
			 }
			 return; // 該当のWBPが見つかれば処理を終了
		 }
	 }
 
	 // 見つからなかった場合は表示しない
	 UE_LOG(LogTemp, Warning, TEXT("対応するWBPが見つかりませんでした。"));
}
// カギを使用できるか判定
bool UDoorKeyManager::CanUseKey(FName doorTag) const
{
    // 取得済みのカギがなければ false
    if (collectedKeyTags.Num() == 0) return false;

    // ドアのタグから対応するカギのタグを取得
    FName requiredKeyTag = FName(*doorTag.ToString().Replace(TEXT("Door"), TEXT("Key")));

    // 取得済みのカギリストに含まれているか確認
    return collectedKeyTags.Contains(requiredKeyTag);
}

// 扉を開けようとする処理
void UDoorKeyManager::TryOpenDoor()
{
    // プレイヤーのアクターを取得
    AActor* playerActor = GetOwner();
    if (!playerActor) return;

    // "DoorOtherThanStone" タグを持つ全ドアを取得
    TArray<AActor*> doors;
    UGameplayStatics::GetAllActorsWithTag(GetWorld(), "OtherDoor", doors);

    for (AActor* door : doors)
    {
        // プレイヤーがドアとオーバーラップしているか確認
        if (playerActor->IsOverlappingActor(door))
        {
            // ドアのタグをチェック
            for (const FName& tag : door->Tags)
            {
                if (tag != "OtherDoor") // "Door" タグはスキップ
                {
                    // カギを持っているか判定
                    bool hasKey = CanUseKey(tag);
                    bShowMessage = true; // E ボタン表示
                    bCanOpenDoor = hasKey; // 開閉フラグ設定

                    // ログ出力
                    if (hasKey)
                    {
                        UE_LOG(LogTemp, Warning, TEXT("%s のカギで扉を開ける！"), *tag.ToString());
                    }
                    else
                    {
                        UE_LOG(LogTemp, Warning, TEXT("カギがないので開かない！"));
                        isOpenDoor = false; // カギがない場合は isOpenDoor を false に設定
                    }

                    return; // 1つのドアだけ判定したら抜ける
                }
            }
        }
    }
}
