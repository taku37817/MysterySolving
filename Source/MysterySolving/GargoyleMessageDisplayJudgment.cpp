// Fill out your copyright notice in the Description page of Project Settings.


#include "GargoyleMessageDisplayJudgment.h"
#include "Blueprint/UserWidget.h"        // ← 必須！（CreateWidget, UUserWidget）
#include "Components/WidgetComponent.h"  // ← 使ってたら（使ってなければ不要）
#include "Kismet/GameplayStatics.h" // ← これを cpp に追加！
#include "EngineUtils.h" // TActorIteratorを使うために必要

// Sets default values for this component's properties
UGargoyleMessageDisplayJudgment::UGargoyleMessageDisplayJudgment()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UGargoyleMessageDisplayJudgment::BeginPlay()
{
	Super::BeginPlay();
	playerActor = FindActorByName(GetWorld(), TEXT("BP_Player")); // プレイヤーアクターを名前で取得
	// ...
	
}


// Called every frame
void UGargoyleMessageDisplayJudgment::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	AActor* gargoyleActor = GetOwner();
	const float distance = FVector::Dist(playerActor->GetActorLocation(), gargoyleActor->GetActorLocation());
	// Gargoyle.cpp (またはComponent)
	if (distance < messageHiddenDistance)
	{
		if (!StandWidgetInstance && StandWidgetClass)
		{
			StandWidgetInstance = CreateWidget<UUserWidget>(GetWorld(), StandWidgetClass);
			StandWidgetInstance->AddToViewport();
		}
		else if (StandWidgetInstance)
		{
			StandWidgetInstance->SetVisibility(ESlateVisibility::Visible);
		}
	}
	else
	{
		if (StandWidgetInstance)
		{
			StandWidgetInstance->SetVisibility(ESlateVisibility::Hidden);
		}
	}


	// ...
}


// 名前でアクターを検索して返す関数
AActor* UGargoyleMessageDisplayJudgment::FindActorByName(UWorld* world, const FString& actorName)
{
    if (world) // ワールドが有効か確認
    {
        for (TActorIterator<AActor> actorIterator(world); actorIterator; ++actorIterator) // アクターを順に探索
        {
            if (actorIterator->GetName().Contains(actorName)) // 名前が一致するか（部分一致）
            {
                return *actorIterator; // 見つかったら返す
            }
        }
    }
    return nullptr; // なければnullptrを返す
}