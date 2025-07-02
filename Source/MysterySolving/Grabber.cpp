// Fill out your copyright notice in the Description page of Project Settings.


#include "Grabber.h" // Grabberクラスのヘッダー
#include "Engine/World.h" // Grabberクラスのヘッダー
#include "DrawDebugHelpers.h"  // デバッグ描画用のヘルパー
#include "GameFramework/Actor.h" // アクター関連の基礎クラス 
#include "Kismet/GameplayStatics.h" // ゲーム関連の便利な関数
#include "Components/BoxComponent.h" //BoxComponent利用
#include "Components/PrimitiveComponent.h"  //GetOverlappingActorsを使うために必要
#include "FinalMysterySolvingRoomClass.h"

// Sets default values for this component's properties
UGrabber::UGrabber()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	// 3個の カギ分のブール
	bIsKeyGrabbedMessage.Init(false, 3);
	// ...
}


// Called when the game starts
void UGrabber::BeginPlay()
{
	Super::BeginPlay();
	// UPhysicsHandleComponent* PhysicsHandle = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();
	// if(PhysicsHandle != nullptr)
	// {
	// 	UE_LOG(LogTemp, Display, TEXT("Got Pyhsics Handle: %s"), *PhysicsHandle->GetName());
	// }
	// else
	// {
	// 	UE_LOG(LogTemp, Display, TEXT("No Pyhsics Found!"));
	// }
	// ...
	
}


// Called every frame
void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	// UE_LOG(LogTemp, Warning, TEXT("Gargoyle Judge (Tag '%s'): %s, Stand Judge (Tag '%s'): %s"),
	// *gargoyleCheckActorTag.ToString(), gargoyleGrabbedMessageDisplayJudgeVariable ? TEXT("true") : TEXT("false"),
	// *standCheckActorTag.ToString(), standGrabbedMessageDisplayJudgeVariable ? TEXT("true") : TEXT("false"));
	UPhysicsHandleComponent* physicsHandle = GetPhysicsHandle(); // 物理ハンドル取得
	OtherMessagesDisplayJudge();
	// 物理ハンドルが存在して、オブジェクトを持っているか確認
	if (physicsHandle && physicsHandle->GetGrabbedComponent())
	{
		FVector playerLocation = GetComponentLocation(); // プレイヤーの現在地を取得
        FVector grabbedLocation = physicsHandle->GetGrabbedComponent()->GetComponentLocation(); // 掴んでいるオブジェクトの位置を取得
        float currentDistance = FVector::Dist(playerLocation, grabbedLocation); // プレイヤーとオブジェクトの距離を計算
        
        // カメラ前方の位置を計算（カメラの向きを考慮）
        FVector forwardDirection = GetWorld()->GetFirstPlayerController()->PlayerCameraManager->GetActorForwardVector();
        FVector targetLocation = playerLocation + forwardDirection * holdDistance; // プレイヤー前方の位置

        // オブジェクトをスムーズに補間してカメラ前方に移動
        FVector currentLocation = physicsHandle->GetGrabbedComponent()->GetComponentLocation(); // 現在の位置を取得
        FVector newLocation = FMath::VInterpTo(currentLocation, targetLocation, DeltaTime, 100000.0f); // 補間スピードを調整
        
        // 物理ハンドルで位置を設定
        physicsHandle->SetTargetLocation(newLocation); // 新しい位置に設定
		// オブジェクトが掴める最大距離を超えた場合
		if (currentDistance >= maxGrabDistance)
		{
			physicsHandle->ReleaseComponent();  // 最大距離を超えたら掴んでいたオブジェクトを放す
			// 掴んだオブジェクトが離れた際の処理（例：音を鳴らす、エフェクトを出すなど）
			// Example: Play a sound or create an effect when releasing
			// UGameplayStatics::PlaySoundAtLocation(this, ReleaseSound, GetComponentLocation());
		}
	}
		gargoyleGrabbedMessageDisplayJudgeVariable;
		standGrabbedMessageDisplayJudgeVariable;
		// メッセージ表示フラグのログ出力（デバッグ用）
		UE_LOG(LogTemp, Warning, TEXT("gargoyleGrabbedMessageDisplayJudgeVariable: %s"), gargoyleGrabbedMessageDisplayJudgeVariable ? TEXT("true") : TEXT("false"));
		UE_LOG(LogTemp, Warning, TEXT("standGrabbedMessageDisplayJudgeDisplayJudgeVariable: %s"), standGrabbedMessageDisplayJudgeVariable ? TEXT("true") : TEXT("false"));
		playerAndGrabbedDistanceJudge = true;
	
	// if(hitResult.bBlockingHit)
	// {
	// 	// 壁にぶつかったら少し押し戻す
	// 	FVector correction = hitResult.ImpactNormal * 2.0f;  // 押し戻し量は調整
	// 	hitActor->AddActorWorldOffset(correction, true);
	// }
	// ...
}
//持つ
void UGrabber::Grab()
{
	UPhysicsHandleComponent *physicsHandle = GetPhysicsHandle();
	if(physicsHandle == nullptr){
		return;
	}
	hasHit = GetGrabberbleInReach(hitResult);  // 物理ハンドル取得
	if(hasHit){
		hitActor = hitResult.GetActor(); //掴めるアクターを取得
		hitComponent = hitResult.GetComponent(); //掴めるコンポーネントを取得
		if (hitActor->ActorHasTag("Key")) // "Key" タグを持っているか確認
		{ 
			FName KeyTypeTag;

			// KeyTags に含まれるタグを探す
			for (const FName& Tag : KeyTags)
			{
				if (hitActor->ActorHasTag(Tag))
				{
					KeyTypeTag = Tag;
					break;
				}
			}
			//KeyTags に含まれるタグを探す
			if (KeyTypeTag.IsNone()) return;

			// 🔹 DoorKeyManager にカギを渡して登録（管理を一元化！）
			doorKeyManagerComponent = GetOwner()->FindComponentByClass<UDoorKeyManager>();
			if (doorKeyManagerComponent)
			{
				doorKeyManagerComponent->CollectKey(KeyTypeTag);
			}
			// 🔹 取得したら非表示 & 衝突無効化
			hitActor->SetActorHiddenInGame(true); // 非表示
			hitActor->SetActorEnableCollision(false);  // 衝突判定を無効にする
		}
		// ブロンズ像を掴んだ場合のフラグ
		if(hitActor->ActorHasTag("BronzeStatue"))
		{
			isBronzeStatueGrabbed = true;//想像持てるメッセージ表示
		}
		// ロウソクを掴んだ場合のフラグ
		if(hitActor->ActorHasTag("GrabbedCandle"))
		{
			bIsCandleGrabbedMessage = false; //キャンドル持てるメッセージ非表示
		}
		hitComponent->SetSimulatePhysics(true);
		//動きを素早く減少
		hitComponent->SetLinearDamping(0.0f);
		hitComponent->SetAngularDamping(0.0f);
		hitComponent->WakeAllRigidBodies();
		// 回転ロック + 物理的な回転速度をゼロにする
		hitComponent->BodyInstance.bLockXRotation = true; //X軸回転停止
		hitComponent->BodyInstance.bLockYRotation = true; //Y軸回転停止
		hitComponent->BodyInstance.bLockZRotation = true; //Z軸回転停止
		hitActor->Tags.Add("Grabbed");
		hitActor->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
		physicsHandle->GrabComponentAtLocationWithRotation(
			hitComponent,
			NAME_None,
			hitResult.ImpactPoint,
			GetComponentRotation()
		);
	}

}
void UGrabber::Release()
{
	UPhysicsHandleComponent* physicsHandle = GetPhysicsHandle();
	isBronzeStatueGrabbed = false; //想像持てるメッセージ非表示
	bIsCandleGrabbedMessage = true; //キャンドル持てるメッセージ表示
	if(physicsHandle && physicsHandle->GetGrabbedComponent())
	{
		AActor* grabberActor = physicsHandle->GetGrabbedComponent()->GetOwner();
		grabberActor->Tags.Remove("Grabbed");
		physicsHandle->ReleaseComponent();
	}
}
UPhysicsHandleComponent* UGrabber::GetPhysicsHandle() const
{
	UPhysicsHandleComponent* result = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();
	if(result == nullptr)
	{
		UE_LOG(LogTemp, Display, TEXT("Grabber requies a UPhysicsHandleComponent,"));
	}
	return result;
}

bool UGrabber::GetGrabberbleInReach(FHitResult& outHitResult) const
{
	FVector start = GetComponentLocation();
	FVector end = start + GetForwardVector() * maxGetGrabDistance;
	DrawDebugLine(GetWorld(), start, end, FColor::Red);
	DrawDebugSphere(GetWorld(), end, 10, 10, FColor::Blue, false, 5);

	FCollisionShape Sphere = FCollisionShape::MakeSphere(grablRedius);
	return GetWorld()->SweepSingleByChannel(
		outHitResult,
		start, end,
		FQuat::Identity,
		ECC_GameTraceChannel2,
		Sphere);

}

TArray<AActor*> UGrabber::IsActorWithTagDistance(const FName tagCheck)
{
	TArray<AActor*> findActors;
	UGameplayStatics::GetAllActorsWithTag(GetWorld(),tagCheck,findActors);
	return findActors;
}
bool UGrabber::IsActorNearby(FName actorTag, FVector playerLocation)
{
	TArray<AActor*> foundActors = IsActorWithTagDistance(actorTag);
    
    for (AActor* actor : foundActors)
    {
        if (actor && FVector::Dist(playerLocation, actor->GetActorLocation()) < messageHiddenDistance)
        {
            return true;
        }
    }
    return false;
}
void UGrabber::SetGargoyleAndStandMessageDisplayJudge(bool standMessageDisplayJudge,bool gargoyleMessageDisplayJudge)
{
	AActor* playerActor = GetOwner();
    if (playerActor)
    {
        FVector playerLocation = playerActor->GetActorLocation();
        isGarGoyleNear = IsActorNearby(gargoyleCheckActorTag, playerLocation);
        isStandeNear = IsActorNearby(standCheckActorTag, playerLocation);
    }

    // 近くにいるときだけメッセージ表示
    gargoyleGrabbedMessageDisplayJudgeVariable = isGarGoyleNear ? gargoyleMessageDisplayJudge : false;
    standGrabbedMessageDisplayJudgeVariable = isStandeNear ? standMessageDisplayJudge : false;
	UE_LOG(LogTemp, Warning, TEXT("gargoyleGrabbedMessageDisplayJudgeVariable: %s"), gargoyleGrabbedMessageDisplayJudgeVariable ? TEXT("true") : TEXT("false"));
	UE_LOG(LogTemp, Warning, TEXT("gargoyleGrabbedMessageDisplayJudgeVariable: %s"), standGrabbedMessageDisplayJudgeVariable ? TEXT("true") : TEXT("false"));
}

//メッセージ表示非表示させるメソッド
void UGrabber::OtherMessagesDisplayJudge()
{
	UBoxComponent* boxComponent; // プレイヤーの子のBoxComponentでオーバーラップさせる
	boxComponent = Cast<UBoxComponent>(GetOwner()->GetDefaultSubobjectByName(TEXT("Box")));
	if (!boxComponent)
	{
		UE_LOG(LogTemp, Warning, TEXT("Not overlapActor!"));
		return;
	}
	// オーバーラップ中のアクターを格納するための配列
	TArray<AActor*> OverlappingActors;
	boxComponent->GetOverlappingActors(OverlappingActors); // ここでオーバーラップアクターを取得
    for(AActor* overlapActor : OverlappingActors)
	{
		if(!overlapActor){
			UE_LOG(LogTemp, Warning, TEXT("Not overlapActor!"));
			return;}
		if(boxComponent && boxComponent->IsOverlappingActor(overlapActor))
		{
			if(overlapActor->ActorHasTag("SilverKey"))
				bIsKeyGrabbedMessage[0] = true;
			else
				bIsKeyGrabbedMessage[0] = false;
			if(overlapActor->ActorHasTag("GoldKey"))
				bIsKeyGrabbedMessage[1] = true;
			else
				bIsKeyGrabbedMessage[1] = false;
			if(overlapActor->ActorHasTag("BrownKey"))
				bIsKeyGrabbedMessage[2] = true;
			else
				bIsKeyGrabbedMessage[2] = false;
			if(overlapActor->ActorHasTag("BronzeStatue"))
				bIsBronzeStatueGrabbedMessage = true;
			else
				bIsBronzeStatueGrabbedMessage = false;
		}
	}
}