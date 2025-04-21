// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GameOverWidget.generated.h"

/**
 * 
 */
UCLASS()
class MYSTERYSOLVING_API UGameOverWidget : public UUserWidget
{
	GENERATED_BODY()

public:
    UFUNCTION(BlueprintCallable)
    void OnTitleButtonClicked(); // タイトルボタンが押されたときに呼ばれる

    UFUNCTION(BlueprintCallable)
    void OnRestartButtonClicked(); // リスタートボタンが押されたときに呼ばれる
};