// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "InteractableObject.generated.h"

UCLASS()
class MYSTERYSOLVING_API AInteractableObject : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AInteractableObject();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
// 近づいた時にEを表示する用のUI
UPROPERTY(EditAnywhere, Category = "UI")
class UWidgetComponent* PickupWidget;

// 衝突範囲（プレイヤーが近づいたかチェック）
UPROPERTY(EditAnywhere, Category = "Interaction")
class USphereComponent* InteractionSphere;

// プレイヤーが近づいた時に呼ばれる
UFUNCTION()
void OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, 
					UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, 
					bool bFromSweep, const FHitResult& SweepResult);

// プレイヤーが離れた時に呼ばれる
UFUNCTION()
void OnEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, 
				  UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
	

};
