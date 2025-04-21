// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "PhysicsEngine/PhysicsHandleComponent.h"
#include "Components/BoxComponent.h"
#include "DoorKeyManager.h"
#include "Grabber.generated.h"

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class MYSTERYSOLVING_API UGrabber : public USceneComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UGrabber();

	UFUNCTION(BlueprintCallable)
	void Release();

	UFUNCTION(BlueprintCallable)
	void Grab();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	//スイッチ台関連
	void SetGargoyleAndStandMessageDisplayJudge(bool standMessageDisplayJudge,bool gargoyleMessageDisplayJudge);
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MessageJudge")
	bool standGrabbedMessageDisplayJudgeVariable = false; //スイッチ台のメッセージ表示・非表示判定（TriggerComponentの変数「standGrabbedJudge」で渡された真偽値取得）
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MessageJudge")
	bool gargoyleGrabbedMessageDisplayJudgeVariable = false; //Gargoyleメッセージ表示・非表示判定（TriggerComponentの変数「standGrabbedJudge」で渡された真偽値取得）
	
    TArray<AActor*> collectedKeysActors;//収集したカギを保存する配列
	bool isBronzeStatueGrabbed = false; //銅像持っているか
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<bool> bIsKeyGrabbedMessage; //カギ持てるかメッセージ表示判定3つ分
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bIsBronzeStatueGrabbedMessage = false; //銅像が持てるかメッセージ表示判定
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bIsCandleGrabbedMessage = false; //キャンドル持てるかメッセージ表示判定

private:
	UPrimitiveComponent* hitComponent;
	UPROPERTY(EditAnywhere)
	float maxGrabDistance = 450; //掴める最大の距離
	UPROPERTY(EditAnywhere)
	float maxGetGrabDistance = 100; //取得できる最大距離
	UPROPERTY(EditAnywhere)
	float grablRedius = 100; //掴める範囲の半径
	UPROPERTY(EditAnywhere)
	float holdDistance = 200;
	UPROPERTY(EditAnywhere)
	FName setDelActorTag;
	bool GetGrabberbleInReach(FHitResult& outHitResult) const; //掴めるアクター取得
	UPhysicsHandleComponent* GetPhysicsHandle() const;
	float initialZ = 0.0f; //持ち上げ開始時の座標
	//ヒット時の取得関連
	FHitResult hitResult;
	bool hasHit;
	AActor* hitActor;
	UBoxComponent* ovarlapBox;
	

	UPROPERTY(EditAnywhere)
	float messageHiddenDistance = 450.0f;
	bool playerAndGrabbedDistanceJudge = false; //プレイヤーと持てるアクターとの距離判定止める

	FName gargoyleCheckActorTag = "GargoyleTag"; //Gargoyleのアクタータグ「GargoyleTag」を検索するための変数
	FName standCheckActorTag = "StandTag"; //Standのアクタータグ「StandTag」を検索するための変数
	FName keyCheckActorTag = "Key";
	TArray<AActor*> IsActorWithTagDistance(const FName tagCheck); //指定したタグがあるアクターとプレイヤーとの距離計算
	bool isGarGoyleNear = false; //Gargoyleが近くにいるかどうか判定用
	bool isStandeNear = false; //Standが近くにいるかどうか判定用
	bool IsActorNearby(FName actorTag, FVector playerLocation); //距離のメッセージ表示できる距離判定メソッド

	UDoorKeyManager* doorKeyManagerComponent;
	UPROPERTY(EditAnywhere)
	TArray<FName> KeyTags = { "GoldKey", "SilverKey", "BrownKey" };
	void OtherMessagesDisplayJudge(); //メッセージ表示非表示させるメソッド
};