// Fill out your copyright notice in the Description page of Project Settings.


#include "CandleAndPointLightOn.h"
#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"           // エフェクトをスポーンしたりしたい時に必要
#include "Components/PointLightComponent.h"

// Sets default values for this component's properties
UCandleAndPointLightOn::UCandleAndPointLightOn()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
}

// Called when the game starts
void UCandleAndPointLightOn::BeginPlay()
{
	Super::BeginPlay();
}


// Called every frame
void UCandleAndPointLightOn::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
    CandleAndPointLightOn();
}
//キャンドルとポイントライト点灯
void UCandleAndPointLightOn::CandleAndPointLightOn()
{
    AActor* candleActor = GetOwner();
    if(!candleActor) 
    {
        UE_LOG(LogTemp, Warning, TEXT("❌Not candleActor!"));
        return;
    }
    if(IsOverlappingActor(grabbedCandleActor))
    {
        if(candleActor->GetName().Contains("Candle"))
        {
            if (UNiagaraComponent* niagaraComp = candleActor->FindComponentByClass<UNiagaraComponent>())
            {
                if(!niagaraComp)
                {
                    UE_LOG(LogTemp, Warning, TEXT("❌Not niagaraComp!"));
                    return;
                }
                niagaraComp->SetVisibility(true);
            }
        }
        if(pointLightActor)
        {
            if (UPointLightComponent* lightComp = pointLightActor->FindComponentByClass<UPointLightComponent>())
            {
                if(!lightComp)
                {
                    UE_LOG(LogTemp, Warning, TEXT("❌Not lightComp!"));
                    return;
                }
                lightComp->SetVisibility(true);
            }
            UE_LOG(LogTemp, Display, TEXT("🕯️ Candle lit!"));
        }
    }
}