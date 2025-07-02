// Fill out your copyright notice in the Description page of Project Settings.

#include "StandMessageDisplayJudgment.h"
#include "TriggerComponent.h"
#include "Grabber.h" // Grabberクラスのヘッダー
#include "Kismet/GameplayStatics.h" // ← これを cpp に追加！
#include "EngineUtils.h" // TActorIteratorを使うために必要

// Sets default values for this component's properties
UStandMessageDisplayJudgment::UStandMessageDisplayJudgment()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UStandMessageDisplayJudgment::BeginPlay()
{
	Super::BeginPlay();
	playerActor = FindActorByName(GetWorld(), TEXT("BP_Player")); // プレイヤーアクターを名前で取得

	// ...
	
}


// Called every frame
void UStandMessageDisplayJudgment::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
    // if (playerActor)
    // {
    //     FName actorNameAsTag = FName("BP_Player");
    //     AActor* standActor = GetOwner();
    //     bool isStandleNear;
    //     if(!standActor) return;
    //     UTriggerComponent* triggerComponent = standActor->FindComponentByClass<UTriggerComponent>();
    //     UGrabber* grabber = playerActor->FindComponentByClass<UGrabber>();
    //     if(triggerComponent->gargoyleOvarlapJudge)
    //     {
    //         FVector standActorLocation = standActor->GetActorLocation();
    //         isStandleNear = IsActorNearby(actorNameAsTag, standActorLocation);
    //         if(isStandleNear)
    //         {
    //             isStandMessageDisplay = true;
    //             grabber->standGrabbedMessageDisplayJudgeVariable = true;
    //         }
    //         else
    //         {
    //             isStandMessageDisplay = false;
    //             grabber->standGrabbedMessageDisplayJudgeVariable = false;
    //         }
    //     }
    // }
    if (!playerActor) return;

	AActor* standActor = GetOwner();
	if (!standActor) return;

	UTriggerComponent* triggerComponent = standActor->FindComponentByClass<UTriggerComponent>();
	if (!triggerComponent) return;

	const bool bHasGargoyle = triggerComponent->gargoyleOvarlapJudge;

	if (bHasGargoyle)
	{
		const float distance = FVector::Dist(playerActor->GetActorLocation(), standActor->GetActorLocation());
		const bool shouldShow = distance < messageHiddenDistance;

		UpdateWidgetVisibility(shouldShow);
	}
	else
	{
		// スタンドに像がないときは必ず非表示
		UpdateWidgetVisibility(false);
	}
	// ...
}

void UStandMessageDisplayJudgment::UpdateWidgetVisibility(bool bVisible)
{
	if (bVisible)
	{
		if (!StandWidgetInstance && StandWidgetClass)
		{
			StandWidgetInstance = CreateWidget<UUserWidget>(GetWorld(), StandWidgetClass);
			if (StandWidgetInstance)
			{
				StandWidgetInstance->AddToViewport();
			}
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
}

// 名前でアクターを検索して返す関数
AActor* UStandMessageDisplayJudgment::FindActorByName(UWorld* world, const FString& actorName)
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