// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/BoxComponent.h"
#include "DoorMover.h"
#include "StoneSpecialDoorMover.generated.h"

/**
 * 
 */
UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class MYSTERYSOLVING_API UStoneSpecialDoorMover : public UBoxComponent
{
	GENERATED_BODY()

public:
	UStoneSpecialDoorMover();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:
	//特殊ドア開閉関連
	void SpecialDoorMover(); //Gargoyle台の指定タグが触れたら指定のStroneGateが開閉する関数
	bool isStandSwitchJudge = true; //ドア開閉処理が連続で行われないように判定
	bool isNotGrabbedStandSwitchJudge = true; //持ってる時のドア開閉処理が連続で行われないように判定
	UPROPERTY(EditAnywhere)
	TArray<AActor*> doorActors; //storneGateアクター
	UPROPERTY(EditAnywhere)
	FName standTag = "StandRed"; //スタンドにStandRedがあるか比較用変数

	UBoxComponent* standPosition;
	//アクターの取得
	AActor* GetAcceptableActor();
	AActor* getAcceptableAtor;
};
