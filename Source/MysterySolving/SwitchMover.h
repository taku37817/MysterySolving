// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SwitchMover.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class MYSTERYSOLVING_API USwitchMover : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	USwitchMover();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;


	// コリジョン判定メソッド
	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor,
				UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);
	// StandPushJudgeの状態を変更する関数
	void SetStandPushJudge(bool isPushing);
	

private:
	// メッシュコンポーネントへの参照
	UPROPERTY()
	UStaticMeshComponent* meshComponent;

	// 押された方向
	FVector pushDirection;

	// 押しているか判定
	bool standPushJudge = false;

	// 2m以内に床以外の障害物があるか判定
	bool IsObstacleNearby(const FVector& Offset);

};
