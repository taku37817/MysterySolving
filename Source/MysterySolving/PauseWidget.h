// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PauseWidget.generated.h"

/**
 * 
 */
UCLASS()
class MYSTERYSOLVING_API UPauseWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
	// virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	UFUNCTION(BlueprintCallable)
	void OnTitleButtonClicked(); // タイトルボタンが押されたときに呼ばれる

	UFUNCTION(BlueprintCallable)
	
	void OnBackButtonClicked(); // 再開ボタン押されたときに呼ばれる

	UFUNCTION(BlueprintCallable)
	void OnRestartButtonClicked(); // リスタートボタンが押されたときに呼ばれる

	UFUNCTION(BlueprintCallable)
	void OnSettingButtonClicked(); // 設定画面ボタンを押されたときに呼ばれる
};
