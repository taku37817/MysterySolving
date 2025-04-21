// Fill out your copyright notice in the Description page of Project Settings.


#include "TriggerComponent.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/Actor.h" // GetOwner() を使うために必要
#include "Engine/World.h"
#include "EngineUtils.h" // TActorIteratorを使用するために必要
#include "Kismet/GameplayStatics.h"

UTriggerComponent::UTriggerComponent()
{
    PrimaryComponentTick.bCanEverTick = true;
}
// Called when the game starts
void UTriggerComponent::BeginPlay()
{
	Super::BeginPlay();
    UE_LOG(LogTemp, Display, TEXT("BeginPlay OK"));
    grabberChannel = ECC_GameTraceChannel2;
    playerActor = FindActorByName(GetWorld(), TEXT("BP_Player")); //WorldにBP_Playerが存在したらplayerActorに代入
    World = GetWorld();  // ★ ここで World を取得
    if (!World)
    {
        UE_LOG(LogTemp, Error, TEXT("UTriggerComponent: GetWorld() returned nullptr!"));
    }
    
}
void UTriggerComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
    // UE_LOG(LogTemp, Display, TEXT("isItemSwitchJudge: %s"), isItemSwitchJudge ? TEXT("True") : TEXT("False"));
    // UE_LOG(LogTemp, Display, TEXT("getAcceptableAtor: %s"), getAcceptableAtor ? TEXT("Valid") : TEXT("nullptr"));
    meshComponent = GetOwner()->FindComponentByClass<UStaticMeshComponent>(); //自分自身のメッシュコンポーネント取得
    World = GetWorld();
    for(AActor* doorActor : doorActors) //関連ドアアクターをループ処理
    {
        doorComponent = doorActor->FindComponentByClass<UDoorMover>(); //ドアコンポーネントを取得
        if(doorComponent != nullptr) //ドアコンポーネントが存在する場合
        {
            doorComponent->StorneDoorMoverFunction(DeltaTime);
        }
    }
    if(meshComponent)
    {
        meshComponent->SetUseCCD(true);
    }
    OvarlapFunction();
    NotOvarlapFunction();
    GrabbedMessageDisplayJudgeFunction();
    SetCollisionForTaggedActors(setCollisionForTaggedActorsTag);
    CheckActorTagsAndMultipleConditionsDoorMoveJudge();
    meshComponent->SetCollisionResponseToChannel(grabberChannel,standGrabbedJudge ? ECR_Block : ECR_Ignore);
    
    //      switchStandComponent = GetOwner()->FindComponentByClass<USwitchMover>(); //SwitchMoverを取得
//     if(switchStandComponent != nullptr) //SwitchMoverが存在していたら
//     {
//         switchStandComponent->SetStandPushJudge(standGrabbedJudge); //SwitchMoverのスイッチ台を押す動作オン・オフ切り替え
//     }
}
void UTriggerComponent::OvarlapFunction()
{
    TArray<AActor*> actors; //オーバーラップ中のアクターを格納する配列
    GetOverlappingActors(actors); //オーバーラップしているアクターを取得
    getAcceptableAtor = GetAcceptableActor(); //許容するアクターを取得
    FVector standLocation = FVector::ZeroVector; //スタンドの座標を取得
    AActor*standActor = GetOwner();  //このコンポーネントが付いているアクターがスタンド
    
    if(standActor) //許容するアクターが存在する場合
    {
        standLocation = standActor->GetActorLocation(); //スタンドの座標を取得
    }
    
    //ドアの開閉を行うための処理
    if(getAcceptableAtor != nullptr) //許容アクターが存在する場合
    {
        // オーバーラップ中の処理
        for(AActor* actor : actors)  //オーバーラップしているアクターをループ処理
        {
            if(IsOverlappingActor(actor) && isItemSwitchJudge)  //オーバーラップしているアクターをループ処理
            {
                meshComponent->SetSimulatePhysics(true);
                meshComponent->SetEnableGravity(true);
                meshComponent->SetCollisionResponseToChannel(ECC_WorldDynamic,ECR_Block);
                meshComponent->SetCollisionResponseToChannel(ECC_WorldStatic,ECR_Block);
                standGrabbedJudge = true;
                GargoyleOvarlapJudge(true); //StoneSpecialDoorMoverコンポネントのStoneGateの開閉条件でしよう
                for(AActor* doorActor : doorActors)  //関連ドアアクターをループ処理
                {
                    doorComponent = doorActor->FindComponentByClass<UDoorMover>();  //ドアコンポーネントを取得
                    if(doorComponent != nullptr)  //ドアコンポーネントが存在する場合
                    {
                        doorComponent->UDoorMover::StartTimer(); //ドアタイマーメソッド開始
                        doorComponent->SetShouldMove(!doorComponent->shouldMove); //DoorMoverのbool変数shouldMoveの切り替え
                    }
                }
                isItemSwitchJudge = false; //ドアスイッチ判定を無効にする
            }
            if(IsOverlappingActor(actor)) //接触時に取得したアクターがトリガーコリジョンに触れているか
            {
                if(actor && actor != standActor && actor->ActorHasTag(acceptableActorTag)) //自分自身は除外かつアクタータグが一致したら
                {
                    gargoylePosition = Cast<UBoxComponent>(standActor->GetDefaultSubobjectByName(TEXT("GargoylePosition"))); //ボックスコリジョンを取得
                    if(gargoylePosition)
                    {
                        FVector targetLocation = gargoylePosition->GetComponentLocation();  // ボックスの位置を取得
                        //ボックスの位置にする
                        FVector newLocation = FVector(targetLocation.X, targetLocation.Y, targetLocation.Z);
                        actor->SetActorLocation(newLocation, false, nullptr, ETeleportType::TeleportPhysics);
                    }
                }
            }
        }
    }
}
void UTriggerComponent::NotOvarlapFunction()
{
    TArray<AActor*> actors; //オーバーラップ中のアクターを格納する配列
    GetOverlappingActors(actors); //オーバーラップしているアクターを取得
    getAcceptableAtor = GetAcceptableActor(); //許容するアクターを取得
    FVector standLocation = FVector::ZeroVector; //スタンドの座標を取得
    //許容アクターが存在しない場合
    if(!getAcceptableAtor)
    {
        meshComponent->SetSimulatePhysics(false);
        meshComponent->SetEnableGravity(false);
        meshComponent->SetCollisionResponseToChannel(ECC_WorldDynamic,ECR_Overlap);
        meshComponent->SetCollisionResponseToChannel(ECC_WorldStatic,ECR_Overlap);
        GargoyleOvarlapJudge(false); //StoneSpecialDoorMoverコンポネントのStoneGateの開閉条件でしよう
        standGrabbedJudge = false; //SwitchMoverのSetStandPushJudge(bool)の動作がfalse
        for(AActor* actor : actors)
        {
            if(grabberComponent && playerActor)
            {
                grabberComponent->SetGargoyleAndStandMessageDisplayJudge(false,true);
            }
            if(!isItemSwitchJudge) //ドアスイッチ判定を無効だった場合
            {
                for(AActor* doorActor : doorActors) //関連ドアアクターをループ処理
                {
                    doorComponent = doorActor->FindComponentByClass<UDoorMover>(); //ドアコンポーネントを取得
                    if(doorComponent != nullptr) //ドアコンポーネントが存在する場合
                    {
                        doorComponent->UDoorMover::StartTimer(); //ドアタイマーメソッド開始
                        doorComponent->SetShouldMove(!doorComponent->shouldMove);  //DoorMoverのbool変数shouldMoveの切り替え
                    }
                }
                isItemSwitchJudge = true; //ドアスイッチ判定を有効
            }
        }
    }
}

void UTriggerComponent::GrabbedMessageDisplayJudgeFunction()
{
    TArray<AActor*> actors; //オーバーラップ中のアクターを格納する配列
    GetOverlappingActors(actors); //オーバーラップしているアクターを取得
    getAcceptableAtor = GetAcceptableActor(); //許容するアクターを取得
    if(getAcceptableAtor)
    {
        for(AActor* actor : actors)
        {
            if (IsOverlappingActor(actor)) // 持てるようになる条件
            {
                if(playerActor)
                {
                    grabberComponent = playerActor->FindComponentByClass<UGrabber>();
                    UE_LOG(LogTemp, Warning, TEXT("Player actor found: %s"), *playerActor->GetName());
                }
                else
                {
                    UE_LOG(LogTemp, Warning, TEXT("Player actor not found"));
                }
                if (grabberComponent && IsOverlappingActor(playerActor))
                {
                    grabberComponent->SetGargoyleAndStandMessageDisplayJudge(true, false);
                    UE_LOG(LogTemp, Warning, TEXT("standMessageDisplayJudg Treu!"));
                }
                else
                {
                    grabberComponent->SetGargoyleAndStandMessageDisplayJudge(false, false);
                }
            }
        }
    }
    else
    {
        if (grabberComponent && playerActor)
        {
            grabberComponent->SetGargoyleAndStandMessageDisplayJudge(false, true);
        }
    }
}

void UTriggerComponent::CheckActorTagsAndMultipleConditionsDoorMoveJudge()
{
    TArray<AActor*> actors; // オーバーラップ中のアクターを格納する配列
    GetOverlappingActors(actors); // オーバーラップしているアクターを取得
    getAcceptableAtor = GetAcceptableActor(); // 許容するアクターを取得

    if (getAcceptableAtor != nullptr)
    {
        for (AActor* actor : actors)
        {
            if (IsOverlappingActor(actor))
            {
                // 取得したアクターのタグ一覧を取得
                TArray<FName> actorTags = actor->Tags;

                // 指定したタグ（UnlockDoor_Red）があるかチェック
                FName foundTag = NAME_None;
                for (const FName& tag : actorTags)
                {
                    if (tag == acceptableActorTag)  // 一致するタグを発見
                    {
                        foundTag = tag;
                        break;  // 最初に見つけたらループを抜ける
                    }
                }

                if (foundTag != NAME_None)
                {
                    getMultipleConditioDoorMoverPrivateActor = actor;
                    // 一致したタグをログに出力
                    UE_LOG(LogTemp, Display, TEXT("✅ Found Matching Tag: %s"), *foundTag.ToString());
                    break; // 最初に見つけたアクターで処理を止める
                }

            }
        }
    }
    else
    {
        getMultipleConditioDoorMoverPrivateActor = nullptr;
    }
}
// 指定したタグを持つアクターのコリジョンレスポンスを Overlap に設定するメソッド
void UTriggerComponent::SetCollisionForTaggedActors(FName targetTag)
{
    TArray<AActor*> foundActors; // アクターを格納する配列を用意 
    if(World)
    {
        UGameplayStatics::GetAllActorsWithTag(GetWorld(), targetTag, foundActors); // 指定タグを持つアクターを取得  
    }
    
    for (AActor* actor : foundActors) // 取得したアクターをループ処理  
    {  
        if (actor) // アクターが存在するかチェック  
        {  
            UPrimitiveComponent* primitiveComponent = Cast<UPrimitiveComponent>(actor->GetComponentByClass(UPrimitiveComponent::StaticClass())); // コンポーネントを取得しキャスト  
            if (primitiveComponent) // キャストが成功したかチェック  
            {  
                primitiveComponent->SetCollisionResponseToChannel(ECC_PhysicsBody, ECR_Overlap); // コリジョンレスポンスを Overlap に設定  
                UE_LOG(LogTemp, Warning, TEXT("%s collision set to Overlap"), *actor->GetName()); // 設定したアクター名をログ出力  
            }  
        }  
    }  
}
//オーバーラップ中のアクターを取得する関数
AActor* UTriggerComponent::GetAcceptableActor()
{
    TArray<AActor*> actors; //オーバーラップしているアクター取得のための配列
    GetOverlappingActors(actors); //オーバーラップしたアクターを取得
    for(AActor* actor : actors)  //アクターごとにループ処理
    {
        bool hasAcceptableTag = actor->ActorHasTag(acceptableActorTag); //許容タグを持っているか判定
        bool isGrabbed = actor->ActorHasTag("Grabbed"); //掴まれているか判定
        if(hasAcceptableTag && !isGrabbed) //許容タグがあり、掴まれていない場合
        {
            return actor; //該当アクターを返す
        }
    }
    return nullptr; //該当アクターがない場合はnullptrを返す
}

// 指定した名前のアクターをワールド内で検索する関数
AActor* UTriggerComponent::FindActorByName(UWorld* world, const FString& actorName)
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

void UTriggerComponent::GargoyleOvarlapJudge(bool argumentGargoyleOvarlapJudge)
{
    gargoyleOvarlapJudge = argumentGargoyleOvarlapJudge;
}
// オーバーラップしたアクターのタグを取得
TArray<FName> UTriggerComponent::GetOverlappingActorTags() const
{
    TArray<FName> tags;

    // オーバーラップしているすべてのアクターを取得
    TArray<AActor*> overlappingActors;
    GetOverlappingActors(overlappingActors);

    for (AActor* actor : overlappingActors)
    {
        if (actor)
        {
            for (const FName& tag : actor->Tags)
            {
                tags.Add(tag);  // FName 単体で追加
            }
        }
    }

    return tags;
}