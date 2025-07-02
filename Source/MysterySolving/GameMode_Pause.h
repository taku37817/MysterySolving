// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Blueprint/UserWidget.h" // ゲーム内で使う UI（UUserWidget など）を扱うため
#include "GameMode_Pause.generated.h"


UCLASS()
class MYSTERYSOLVING_API AGameMode_Pause : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	AGameMode_Pause();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	
	UPROPERTY()
    UInputComponent* InputComp;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
	UFUNCTION(BlueprintCallable)
	void Pause(); // ポーズ画面に遷移
	UFUNCTION(BlueprintCallable)
	void ShowPauseMenu();  // 表示処理

	UFUNCTION(BlueprintCallable)
	void SettingVisible();  // 設定表示処理

	UFUNCTION(BlueprintCallable)
	void GuidVisible();  // ヒント表示処理

	UFUNCTION(BlueprintCallable)
	void OperationGuidVisible();  // 表示処理

private:
	UPROPERTY(EditAnywhere, Category = "UI")
	TSubclassOf<UUserWidget> pauseWidgetClass; //ポーズWBP表示するUI

	/** ポーズUIのインスタンス */
	UUserWidget* pauseOverWidget;
    void HidePauseMenu();       // 非表示処理	
	 // すべてのポーズ系ウィジェット（設定・ヒント・操作）を非表示にする関数
    void ClearAllPauseWidgets(); 


	bool bIsPaused = false; // 現在ポーズ中かどうか
	// --- UIのBlueprintクラス（これをCreateWidgetに使う） ---
	UPROPERTY(EditAnywhere, Category = "UI")
	TSubclassOf<UUserWidget> settingWidgetClass;

	UPROPERTY(EditAnywhere, Category = "UI")
	TSubclassOf<UUserWidget> guidVWidgetClass;

	UPROPERTY(EditAnywhere, Category = "UI")
	TSubclassOf<UUserWidget> operationGuidWidgetClass;

	// --- 実際に作られたウィジェットインスタンス ---
	UPROPERTY()
	UUserWidget* settingWidget = nullptr;

	UPROPERTY()
	UUserWidget* guidWidget = nullptr;

	UPROPERTY()
	UUserWidget* operationGuidWidget = nullptr;

};