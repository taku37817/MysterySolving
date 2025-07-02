// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "GameSettingManager.h"
#include "GetUObjectClass.generated.h"

/**
 * 
 */
UCLASS()
class MYSTERYSOLVING_API UGetUObjectClass : public UGameInstance
{
	GENERATED_BODY()

public:
    virtual void Init() override;

    UFUNCTION(BlueprintCallable)
    UGameSettingManager* GetSettingManager() const { return SettingManager; }

private:
    UPROPERTY()
    UGameSettingManager* SettingManager;
	
};
