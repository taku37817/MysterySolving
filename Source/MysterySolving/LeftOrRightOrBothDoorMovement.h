// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Components/AudioComponent.h"
#include "Components/BoxComponent.h"
#include "GameFramework/Actor.h"
#include "DoorKeyManager.h" // ドアの鍵状態を管理するカスタムコンポーネント
#include "LeftOrRightOrBothDoorMovement.generated.h"


// Forward declarations
class UStaticMeshComponent;
class UAudioComponent;
class UBoxComponent;
class UDoorKeyManager;
class AActor;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class MYSTERYSOLVING_API ULeftOrRightOrBothDoorMovement : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	ULeftOrRightOrBothDoorMovement();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	// UFUNCTION(BlueprintCallable)
	// void LeftDoorOpen();

	// UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Door Movement")
	// AActor* doorActor; // ドアのアクター
	// UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Door Movement")
	// float openDoorRotationZ = -90.f; // 開けるときのZ軸回転角
	// UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Door Movement")
	// float closeDoorRotationZ = 0.0f; // 開けるときのZ軸回転角
	// UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Door Movement")
	// float openSpeed = 2.0f; // 回転スピード

private:

	// プレイヤーアクター
	AActor* playerActor;

	// ドアアクター
	UPROPERTY(EditAnywhere)
	AActor* doorActor;

	// メッシュコンポーネント（複数のドアメッシュを扱う）
	TArray<UStaticMeshComponent*> meshComponents;

	// AudioComponent
	UPROPERTY(EditAnywhere)
	UAudioComponent* audioComponent;

	// BoxComponent（トリガー用）
	UBoxComponent* boxComp;

	// 鍵管理コンポーネント
	UDoorKeyManager* doorKeyManagerComponent;

	// ドアの回転速度
	float openSpeed = 2.0f;

	// 左ドアが開く角度
	UPROPERTY(EditAnywhere)
	float openLeftDoorRotationZ = 90.0f;

	// 右doorが開く角度
	UPROPERTY(EditAnywhere)
	float openRightDoorRotationZ = 90.0f;


	// ドアが閉じる角度
	UPROPERTY(EditAnywhere)
	float closeDoorRotationZ = 0.0f;

	// ドアの初期回転
	FRotator originalRotation;

	// ドアのメッシュごとの初期回転を保存するためのマップ
	TMap<UStaticMeshComponent*, FRotator> originalRotations;
	
	//　ドアの開く動作を一度しかできないようにする
	bool openDoorJudge = true;
	
	//ドアを開ける時間
	float countdown = 0.0f;
	// 左ドアを開ける処理
	void LeftDoorOpen(float deltaTime);

	// 右ドアを開ける処理
	void RightDoorOpen(float deltaTime);
	// ドアを閉じる処理
	void CloseDoor(float deltaTime);

	// ドアのBoxComponentを見つける
	void FindBoxComponent();

	// 名前でアクターを検索して返す関数
	AActor* FindActorByName(UWorld* world, const FString& actorName);
		
};
