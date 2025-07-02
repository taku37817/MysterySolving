// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/BoxComponent.h"
#include "FinalMysterySolvingRoomClass.generated.h"

/**
 * 
 */
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class MYSTERYSOLVING_API UFinalMysterySolvingRoomClass : public UBoxComponent
{
	GENERATED_BODY()
public:

	UFinalMysterySolvingRoomClass();

protected:
	//Called when the game starts
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	bool isCandleGrabbed = false; //Candleを持ってるか

private:
	float deltaTime;
	UPROPERTY(EditAnywhere)
	AActor* bronzeStatueActor; //銅像アクター
	UPROPERTY(EditAnywhere)
	AActor* doorActor; //ドアアクター取得
	UPROPERTY(EditAnywhere)
	TArray<AActor*> candleActors; // ロウソクのアクター複数
	UPROPERTY(EditAnywhere)
	TArray<AActor*> pointLightActors; //ポイントライトのアクター
	UPROPERTY(EditAnywhere)
	AActor* grabbedCandleActor; //持てるロウソクアクター取得
	UBoxComponent* grabberlePosition;
	UWorld* World; //GetWorld()使用するための変数
	FName acceptableActorTag = "BronzeStatue"; //許容タグを持っているか判定(銅像)
	bool bLightingManualOn = true; //手動で点灯可能にさせる
	bool bIsCandleGrabbedMessageOnlyOnce = true; //持てるかキャンドルメッセージのbool判定を適用を一回のみにするための判定
	void CandlEeraseFunction(bool isCandleVisibility); //銅像持ってた場合ナビゲーター(ロウソクの火)を消す
	void StateOfTheCandle(); //キャンドルが点いているか
	void DoorMoveJudgeFunction(bool doorMover); //ドアを動かせるか
	void DoYouHaveBronzeStatueOrPlaced(); //銅像を持ってるかまたは置かれているか
	void GetAcceptableActorFunction(); //オーバーラップ中のアクター取得
	void CandleDoYouHaveFunction(); //Candleを持ってるか
	void OverlapJudgmentFunction(); //オーバーラップしてるか
	AActor* FindActorByName(UWorld* world, const FString& actorName); // 指定した名前のアクターをワールド内で検索する関数
	AActor* GetAcceptableActor(); //オーバーラップ中のアクターを取得する関数
 
};
