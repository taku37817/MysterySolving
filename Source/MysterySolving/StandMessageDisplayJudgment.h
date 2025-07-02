// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "StandMessageDisplayJudgment.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class MYSTERYSOLVING_API UStandMessageDisplayJudgment : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UStandMessageDisplayJudgment();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:
	// プレイヤーアクター*	
	AActor* playerActor;
	// 名前でアクターを検索して返す関数
	AActor* FindActorByName(UWorld* world, const FString& actorName);

	UPROPERTY(EditAnywhere)
	float messageHiddenDistance = 450.0f;
	void SetStandMessageDisplayJudge();
	UPROPERTY(EditAnywhere, Category = "UI")
	TSubclassOf<UUserWidget> StandWidgetClass;
	UUserWidget* StandWidgetInstance = nullptr;

	// 新たに追加するメソッド
 	void UpdateWidgetVisibility(bool bVisible);


};
