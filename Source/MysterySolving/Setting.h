// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Blueprint/UserWidget.h"
#include "Setting.generated.h"

class UGameSettingManager; // 前方宣言：設定マネージャー使うため
/**
 * 
 */
UCLASS()
class MYSTERYSOLVING_API ASetting : public AActor
{
	GENERATED_BODY()

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	ASetting();
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
public:
	UFUNCTION(BlueprintCallable)
	void OnBackButtonClicked(); // 戻るボタンが押されたときに呼ばれる関数

	 //SettingWidgetを取得
	 UPROPERTY(EditAnywhere, Category = "UI")
	 TSubclassOf<UUserWidget> settingWidgetClass;
 
	 /** ゲームオーバーUIのインスタンス */
	 UUserWidget* settingWidget;

	 bool bIsVisible = false;
};
