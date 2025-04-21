// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Blueprint/UserWidget.h" // UUserWidget とそのメソッドを使うために必要
#include "TitleInOperable.generated.h"

UCLASS()
class MYSTERYSOLVING_API ATitleInOperable : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATitleInOperable();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:

	void NotOperation(); // キャラクターの操作を無効にし、UIのみに集中させる処理

	UPROPERTY(EditAnywhere)
	TSubclassOf<UUserWidget> titlWidgetClass; //ポーズWBP表示するUI

	UUserWidget* titleWidget;

};
