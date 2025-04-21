// Fill out your copyright notice in the Description page of Project Settings.


#include "GetUObjectClass.h"

void UGetUObjectClass::Init()
{
    Super::Init();
    SettingManager = NewObject<UGameSettingManager>(this, UGameSettingManager::StaticClass());
    if (SettingManager)
    {
        SettingManager->SaveSettings();
        SettingManager->LoadSettings();
    }
}

