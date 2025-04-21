// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/BoxComponent.h"
#include "Components/ActorComponent.h"
#include "Components/InputComponent.h"
#include "CandleAndPointLightOn.generated.h"

/**
 * 
 */
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class MYSTERYSOLVING_API UCandleAndPointLightOn : public UBoxComponent
{
	GENERATED_BODY()

public:

	UCandleAndPointLightOn();

protected:
	//Called when the game starts
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	
	void CandleAndPointLightOn();

private:
	UPROPERTY(EditAnywhere)
	AActor* grabbedCandleActor; //持てるキャンドル
	UPROPERTY(EditAnywhere)
	AActor* pointLightActor; //消灯点灯するキャンドルのライト
};
