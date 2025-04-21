// Fill out your copyright notice in the Description page of Project Settings.


#include "InteractableObject.h"
#include "Components/SphereComponent.h"
#include "COmponents/WidgetComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Blueprint/UserWidget.h"

// Sets default values
AInteractableObject::AInteractableObject()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
 // 衝突判定を追加
 InteractionSphere = CreateDefaultSubobject<USphereComponent>(TEXT("InteractionSphere"));
 RootComponent = InteractionSphere;
 InteractionSphere->InitSphereRadius(100.0f);  // プレイヤーが近づく範囲
 InteractionSphere->SetCollisionProfileName(TEXT("Trigger"));

 // ウィジェットコンポーネント（Eで拾う表示）
 PickupWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("PickupWidget"));
 PickupWidget->SetupAttachment(RootComponent);
 PickupWidget->SetVisibility(false);  // 最初は非表示

 // 衝突イベントを設定
 InteractionSphere->OnComponentBeginOverlap.AddDynamic(this, &AInteractableObject::OnBeginOverlap);
 InteractionSphere->OnComponentEndOverlap.AddDynamic(this, &AInteractableObject::OnEndOverlap);
}

// Called when the game starts or when spawned
void AInteractableObject::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AInteractableObject::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// プレイヤーが近づいた時の処理
void AInteractableObject::OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, 
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, 
	bool bFromSweep, const FHitResult& SweepResult)
{
if (PickupWidget)
{
PickupWidget->SetVisibility(true);  // Eキーを表示
}
}

// プレイヤーが離れた時の処理
void AInteractableObject::OnEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, 
  UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
if (PickupWidget)
{
PickupWidget->SetVisibility(false);  // Eキーを非表示
}
}
