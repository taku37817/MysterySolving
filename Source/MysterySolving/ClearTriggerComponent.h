// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/BoxComponent.h"
#include "GameFramework/Actor.h" 
#include "Blueprint/UserWidget.h"// ゲーム内で使う UI（UUserWidget など）を扱うため
#include "ClearTriggerComponent.generated.h"

/**
 * 
 */
UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class MYSTERYSOLVING_API UClearTriggerComponent : public UBoxComponent
{
	GENERATED_BODY()
public:
	UClearTriggerComponent();

protected:
    virtual void BeginPlay() override; // ゲーム開始時に呼ばれる

public:
    virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override; // 毎フレーム処理

private:
	void ClearOverlapFunction(); // オーバーラップ判定処理

	UPROPERTY()
	bool bIsCleared = false; // 1回だけ処理するためのフラグ

	public:
	UPROPERTY(EditAnywhere, Category = "UI")
	TSubclassOf<UUserWidget> clearWidgetClass; // クリア時に表示するUI

	UPROPERTY(EditAnywhere)
	AActor* gameModeGameOverActor; // ゲームオーバー管理用アクター

	UPROPERTY(EditAnywhere)
	AActor* bronzeStatueActor; // 対象の銅像アクター
};
