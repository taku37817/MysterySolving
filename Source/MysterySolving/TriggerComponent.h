// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/BoxComponent.h"
#include "DoorMover.h"

#include "TriggerComponent.generated.h"

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class MYSTERYSOLVING_API UTriggerComponent : public UBoxComponent
{
	GENERATED_BODY()

public:
	UTriggerComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	//スイッチ台関連
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stand")
	bool standGrabbedJudge = false; //スイッチ台を持てるか判定
	void GargoyleOvarlapJudge(bool argumentGargoyleOvarlapJudge); //GargoyleがOverlapしているか
	bool gargoyleOvarlapJudge;
	// オーバーラップしたアクターのタグを取得
	TArray<FName> GetOverlappingActorTags() const;

	AActor* getMultipleConditioDoorMoverPrivateActor;
	bool isMultipleConditioDoorMoverPrivateActor = false;



private:
	void OvarlapFunction();
	void NotOvarlapFunction();
	void SetCollisionForTaggedActors(FName targetTag);
	void CheckActorTagsAndMultipleConditionsDoorMoveJudge();
	FName setCollisionForTaggedActorsTag = "GargoyleTag";
	UStaticMeshComponent* meshComponent;
	UPROPERTY(EditAnywhere)
	float sphereRadius = 100.0f;
	UPROPERTY(EditAnywhere)
	TArray<AActor*> doorActors; //storneGa
	// UPROPERTY(EditAnywhere)
	// AActor* gargoyleActor; //オーバーラップさせるガーゴイルの色
	// //持ち運べるアクターのDeltailsを取得するためアクター格納変数
	// UPROPERTY(EditAnywhere)
	// AActor* grabbedActor; //例)持ち運べるものGargoyle_Redだと、 Gargoyle_Redをこれにアタッチ
	UPrimitiveComponent* overlapActorMeshComponent;
	UDoorMover* doorComponent;
	UPROPERTY(EditAnywhere)
	FName acceptableActorTag;
	AActor* GetAcceptableActor();
	AActor* getAcceptableAtor;
	bool isItemSwitchJudge = true;
	// UPROPERTY(EditAnywhere)
	// bool isStandGrabbedJudge = true;
	//ColisionのGrabberトレースを操作するための変数

	ECollisionChannel grabberChannel;
	UBoxComponent* gargoylePosition; //ボックスコリジョン取得変数

	AActor* FindActorByName(UWorld* world, const FString& actorName); //指定した名前のアクターをワールド内で検索する関数
	AActor* playerActor;
	UWorld* World; //GetWorld()使用するための変数

	UPROPERTY(EditAnywhere)
	float messageHiddenDistance = 450.0f;//プレイヤーと持てるアクターとの距離
	UPROPERTY(EditAnywhere)
	AActor* gargoyle;//対象にしたいガーゴイルをアタッチ
};
