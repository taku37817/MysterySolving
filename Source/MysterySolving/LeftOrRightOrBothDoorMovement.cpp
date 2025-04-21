#include "LeftOrRightOrBothDoorMovement.h"
#include "EngineUtils.h" // TActorIteratorを使うために必要
#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"
#include "Components/AudioComponent.h"


// コンストラクタ。Tick処理を有効にする
ULeftOrRightOrBothDoorMovement::ULeftOrRightOrBothDoorMovement()
{
    PrimaryComponentTick.bCanEverTick = true; // 毎フレームTickされるように設定
}

// ゲーム開始時に一度だけ呼ばれる初期化処理
void ULeftOrRightOrBothDoorMovement::BeginPlay()
{
    Super::BeginPlay(); // 親クラスの処理を呼び出す
    FindBoxComponent();
    playerActor = FindActorByName(GetWorld(), TEXT("BP_Player")); // プレイヤーアクターを名前で取得

    if (!doorActor) return; // ドアアクターが設定されていない場合は処理を中断

    doorActor->GetComponents<UStaticMeshComponent>(meshComponents); // StaticMeshComponentをすべて取得

    for (UStaticMeshComponent* meshComponent : meshComponents) // 各メッシュコンポーネントに対して処理
    {
        if (meshComponent && meshComponent->ComponentTags.Contains(FName("LeftMoveDoor")) || // "LeftMoveDoor"タグを持つメッシュがあれば
             meshComponent->ComponentTags.Contains(FName("RightMoveDoor"))) // "RightMoveDoor"タグを持つメッシュがあれば
        {
            originalRotation = meshComponent->GetComponentRotation(); // メッシュの初期回転を保存
            originalRotations.Add(meshComponent, originalRotation); // 各メッシュごとに初期回転を保存
            UE_LOG(LogTemp, Warning, TEXT("Found mesh component with tag: %s"), *meshComponent->GetName()); // ログ出力
        }
    }

    audioComponent = GetOwner()->FindComponentByClass<UAudioComponent>(); // AudioComponentを取得

    if (audioComponent)
    {
        audioComponent->bAutoActivate = false; // 自動再生をオフにする
    }
}

// 毎フレーム呼ばれるTick処理
void ULeftOrRightOrBothDoorMovement::TickComponent(float deltaTime, ELevelTick tickType, FActorComponentTickFunction* thisTickFunction)
{
    Super::TickComponent(deltaTime, tickType, thisTickFunction); // 親クラスのTickを呼ぶ

    if (!playerActor || meshComponents.Num() == 0) return; // プレイヤーまたはメッシュコンポーネントが無ければ処理を中断
    
    doorKeyManagerComponent = playerActor->FindComponentByClass<UDoorKeyManager>(); // プレイヤーから鍵管理コンポーネント取得
    if (!doorKeyManagerComponent) return; // 見つからなければ処理中断

    if(boxComp)
    {
        if(boxComp->IsOverlappingActor(playerActor))
        {
            UE_LOG(LogTemp, Warning, TEXT("Is overlapping: %s"), boxComp->IsOverlappingActor(playerActor) ? TEXT("YES") : TEXT("NO"));
           
            if (doorKeyManagerComponent->isOpenDoor && doorKeyManagerComponent->bCanOpenDoor) // ドアが開く条件を満たしていれば開ける
            {   
                LeftDoorOpen(deltaTime); // ドアを開けるメソッド
                RightDoorOpen(deltaTime);
            }
        }
        else if(!boxComp->IsOverlappingActor(playerActor))
        {   
            CloseDoor(deltaTime);
        }
    }
}
// ドアのBoxComponentを見つける
void ULeftOrRightOrBothDoorMovement::FindBoxComponent()
{
    if (!doorActor) // ドアアクターがnullかチェック
    {
        UE_LOG(LogTemp, Error, TEXT("doorActor is null!"));
        return;
    }

    TArray<UActorComponent*> components; // コンポーネント格納用配列
    doorActor->GetComponents(UBoxComponent::StaticClass(), components); // BoxComponentのみ取得

    for (UActorComponent* component : components) // 各コンポーネントを確認
    {
        if (component->GetName() == "Box") // 名前がBoxか確認
        {
            boxComp = Cast<UBoxComponent>(component); // BoxComponentにキャストして保存
            break;
        }
    }

    if (boxComp) // 見つかったらログ
    {
        UE_LOG(LogTemp, Warning, TEXT("Found Box Component: %s"), *boxComp->GetName());
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("Box Component not found!"));
        return;
    }
}

// ドアを閉じる処理
void ULeftOrRightOrBothDoorMovement::CloseDoor(float deltaTime)
{
    for (UStaticMeshComponent* meshComponent : meshComponents) // 各メッシュコンポーネントをループ
    {
        if (meshComponent && meshComponent->ComponentTags.Contains(FName("LeftMoveDoor")) || // "LeftMoveDoor"タグを持つメッシュがあれば
             meshComponent->ComponentTags.Contains(FName("RightMoveDoor"))) // "RightMoveDoor"タグを持つメッシュがあれば
        {
            FRotator targetRotation = originalRotations[meshComponent]; // 各メッシュの初期回転をターゲット回転として設定
            FRotator newRotation = FMath::RInterpTo(meshComponent->GetComponentRotation(), targetRotation, deltaTime, openSpeed); // 補間を使って回転させる
            meshComponent->SetWorldRotation(newRotation); // 回転を適用
        }
    }
    // ドア音の再生
    if (audioComponent && !audioComponent->IsPlaying()) // 音が再生されていなければ再生
    {
        audioComponent->Play();
    }
}

// 左側のドアを開ける処理
void ULeftOrRightOrBothDoorMovement::LeftDoorOpen(float deltaTime)
{
    for (UStaticMeshComponent* meshComponent : meshComponents) // 各メッシュコンポーネントをループ
    {
        if (meshComponent && meshComponent->ComponentTags.Contains(FName("LeftMoveDoor"))) // "MoveDoor"タグを持つメッシュがあれば
        {
            FRotator targetRotation = FRotator(originalRotations[meshComponent].Pitch, originalRotations[meshComponent].Yaw + openLeftDoorRotationZ, originalRotations[meshComponent].Roll); // 開けるためのターゲット回転
            FRotator newRotation = FMath::RInterpTo(meshComponent->GetComponentRotation(), targetRotation, deltaTime, openSpeed); // 補間して回転を計算
            meshComponent->SetWorldRotation(newRotation); // 計算した回転を適用
        }
    }

    // ドア音の再生
    if (audioComponent && !audioComponent->IsPlaying()) // 音が再生されていなければ再生
    {
        audioComponent->Play();
    }
}

// 右ドアを開ける処理
void ULeftOrRightOrBothDoorMovement::RightDoorOpen(float deltaTime)
{
    for (UStaticMeshComponent* meshComponent : meshComponents) // 各メッシュコンポーネントをループ
    {
        if (meshComponent && meshComponent->ComponentTags.Contains(FName("RightMoveDoor"))) // "MoveDoor"タグを持つメッシュがあれば
        {
            FRotator targetRotation = FRotator(originalRotations[meshComponent].Pitch, originalRotations[meshComponent].Yaw + openRightDoorRotationZ, originalRotations[meshComponent].Roll); // 開けるためのターゲット回転
            FRotator newRotation = FMath::RInterpTo(meshComponent->GetComponentRotation(), targetRotation, deltaTime, openSpeed); // 補間して回転を計算
            meshComponent->SetWorldRotation(newRotation); // 計算した回転を適用
        }
    }

    // ドア音の再生
    if (audioComponent && !audioComponent->IsPlaying()) // 音が再生されていなければ再生
    {
        audioComponent->Play();
    }
}

// 名前でアクターを検索して返す関数
AActor* ULeftOrRightOrBothDoorMovement::FindActorByName(UWorld* world, const FString& actorName)
{
    if (world) // ワールドが有効か確認
    {
        for (TActorIterator<AActor> actorIterator(world); actorIterator; ++actorIterator) // アクターを順に探索
        {
            if (actorIterator->GetName().Contains(actorName)) // 名前が一致するか（部分一致）
            {
                return *actorIterator; // 見つかったら返す
            }
        }
    }
    return nullptr; // なければnullptrを返す
}