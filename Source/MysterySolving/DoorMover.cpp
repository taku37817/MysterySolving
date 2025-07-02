// Fill out your copyright notice in the Description page of Project Settings.


#include "DoorMover.h"  // このクラスのヘッダーファイルをインクルード
#include "Engine/World.h"  // ワールド関連の関数やクラスにアクセスするため
#include "TimerManager.h"  // タイマーを管理するために必要
#include "Math/UnrealMathUtility.h"  // 数学系ユーティリティ（例：FMath）を使うため


// Sets default values for this component's properties
UDoorMover::UDoorMover()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	//audioComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("AudioComponent"));
	// ...
}


// Called when the game starts
void UDoorMover::BeginPlay()
{
	Super::BeginPlay();
	audioComponent = GetOwner()->FindComponentByClass<UAudioComponent>();

	originalLocation = GetOwner()->GetActorLocation(); // ドアの元の位置を取得

	openDoorOfset = FVector(originalLocation.X,originalLocation.Y,originalLocation.Z + openDoorTransformLocationZ); // ドアが開いた位置の計算
	closeDoorOfset = FVector(originalLocation.X,originalLocation.Y,originalLocation.Z + closeDoorTransformLocationZ); // ドアが閉じた位置の計算
	if(audioComponent) // AudioComponentが取得できた場合
	{
		audioComponent->bAutoActivate = false;  // 自動で再生されないように設定
	}
	time = GetWorld()->GetTimeSeconds();  // ゲーム開始時の時間を保存（未使用かも）
	// ...
	
}


// Called every frame
void UDoorMover::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	// if(FMath::IsNearlyEqual(currentLocation.Z,150.0f,0.1f) || 
	// 	FMath::IsNearlyEqual(currentLocation.Z,-150.0f,0.1f))
	// {
	// 	if(audioComponent && audioComponent->IsPlaying())
	// 	{
	// 		audioComponent->Stop();
	// 	}
	// }
	//StorneDoorMoverFunction(DeltaTime);
	if (!audioComponent) // AudioComponentが設定されていない場合のエラー確認
    {
        UE_LOG(LogTemp, Error, TEXT("AudioComponent is NULL!"));
    }
    if (!soundToPlay) // Soundアセットが設定されていない場合のエラー確認
    {
        UE_LOG(LogTemp, Error, TEXT("SoundToPlay is NULL!"));
    }
	// ...
}
void UDoorMover::SetShouldMove(bool openClose) // ドアを開ける／閉じるかのフラグを外部から設定する関数
{
	shouldMove = openClose;
}
void UDoorMover::StorneDoorMoverFunction(float deltaTime) // ドアを移動させる関数（滑らかに動かす）

{
	targetLocation = shouldMove 
					? openDoorOfset = FVector(originalLocation.X,originalLocation.Y,openDoorTransformLocationZ)  // 開くときの位置
					: closeDoorOfset = FVector(originalLocation.X,originalLocation.Y,closeDoorTransformLocationZ); // 閉じるときの位置
	FVector currentLocation = GetOwner()->GetActorLocation(); // 現在の位置を取得
	speed = (targetLocation - currentLocation).Length() / moveTime; // 移動速度を計算

	FVector newLocation = FMath::VInterpConstantTo(currentLocation,targetLocation,deltaTime,speed); // 定速で補間移動
	GetOwner()->SetActorLocation(newLocation); // アクターの位置を更新
}
void UDoorMover::StartTimer()
{
	isTimerActive = true;
	GetWorld()->GetTimerManager().SetTimer(timerHandle,this,&UDoorMover::ResetTimer,5.0f,false);  // 5秒後にResetTimerを呼び出す
	if(audioComponent && soundToPlay && !audioComponent->IsPlaying())  // 音が再生されていないなら
	{
		if(isTimerActive) // タイマーがアクティブなら音を再生
		{
			audioComponent->SetSound(soundToPlay); // 再生する音をセット
			audioComponent->Play();
			UE_LOG(LogTemp, Warning, TEXT("Audio should now be playing!"));
		}
	}
	UE_LOG(LogTemp, Warning, TEXT("タイマー開始！ 5秒カウント中"));
}
void UDoorMover::ResetTimer()  // タイマー終了時に呼ばれる関数
{
	timerValue = 0.0f;  // タイマー値をリセット
	isTimerActive = false;  // タイマーを無効にする

	if(audioComponent && soundToPlay && audioComponent->IsPlaying())  // 音が再生中の場合
	{
		if(!isTimerActive)  // タイマーが無効なら停止
		{
			audioComponent->SetSound(soundToPlay);
			audioComponent->Stop();  // 音を止める
			UE_LOG(LogTemp, Warning, TEXT("Audio should now be NoPplaying!"));
		}
	}
	UE_LOG(LogTemp, Warning, TEXT("タイマーリセット！"));  // デバッグログ
}

