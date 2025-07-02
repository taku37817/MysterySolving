// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "TriggerComponent.h"
#include "DoorMover.h"

#include "MultipleConditioDoorMover.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class MYSTERYSOLVING_API UMultipleConditioDoorMover : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UMultipleConditioDoorMover();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
private:

	UPROPERTY(EditAnywhere)
	TArray<AActor*> triggerComponentActors;
	UTriggerComponent* triggerComponent;  // TriggerComponentの参照

	// 比較用タグの配列
	TArray<FName> hikakuTag;

	//DeltaTime取得
	float deltaTime;
	
	// 条件に一致するかをチェックするメソッド
	void CheckConditions();
};
