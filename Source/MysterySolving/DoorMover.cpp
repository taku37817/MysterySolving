// Fill out your copyright notice in the Description page of Project Settings.


#include "DoorMover.h"  // このクラスのヘッダーファイル
#include "Engine/World.h"  // ワールド関連の処理に必要
#include "TimerManager.h"  // タイマー処理を扱うためのヘッダー
#include "Math/UnrealMathUtility.h"  // FMathなどの数学関数を使うため


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
	Super::BeginPlay();  // 親クラスのBeginPlayも実行

	audioComponent = GetOwner()->FindComponentByClass<UAudioComponent>();  // オーナーからAudioComponentを取得

	originalLocation = GetOwner()->GetActorLocation();  // アクターの初期位置を取得
	openDoorOfset = FVector(originalLocation.X, originalLocation.Y, originalLocation.Z + openDoorTransformLocationZ);  // ドアが開いたときの位置を計算
	closeDoorOfset = FVector(originalLocation.X, originalLocation.Y, originalLocation.Z + closeDoorTransformLocationZ);  // ドアが閉じたときの位置を計算

	if(audioComponent)  // AudioComponentが存在する場合
	{
		audioComponent->bAutoActivate = false;  // 自動再生を無効にする（明示的に再生する）
	}

	time = GetWorld()->GetTimeSeconds();  // ゲーム開始時の時間を取得（未使用かも）
	// ...
}



// Called every frame
void UDoorMover::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);  // 親クラスのTick処理を実行

	// if文ブロック（不要な動作停止処理。現在は無効化中）
	// if(FMath::IsNearlyEqual(currentLocation.Z,150.0f,0.1f) || 
	// 	FMath::IsNearlyEqual(currentLocation.Z,-150.0f,0.1f))
	// {
	// 	if(audioComponent && audioComponent->IsPlaying())
	// 	{
	// 		audioComponent->Stop();
	// 	}
	// }

	//StorneDoorMoverFunction(DeltaTime);  // 毎フレームのドア移動処理（コメントアウト中）

	if (!audioComponent)  // AudioComponentが未設定の場合
    {
        UE_LOG(LogTemp, Error, TEXT("AudioComponent is NULL!"));  // エラーログ出力
    }

    if (!soundToPlay)  // 再生すべきサウンドが未設定の場合
    {
        UE_LOG(LogTemp, Error, TEXT("SoundToPlay is NULL!"));  // エラーログ出力
    }

	// ...
}

// ドアを開けるか閉めるかのフラグをセット
void UDoorMover::SetShouldMove(bool openClose)  
{
	shouldMove = openClose;  // フラグを外部から受け取って保持
}

// ドアを滑らかに移動させる処理
void UDoorMover::StorneDoorMoverFunction(float deltaTime)  
{
	targetLocation = shouldMove  // ドアを開くか閉めるかでターゲット位置を設定
		? openDoorOfset = FVector(originalLocation.X, originalLocation.Y, openDoorTransformLocationZ)  // 開く位置
		: closeDoorOfset = FVector(originalLocation.X, originalLocation.Y, closeDoorTransformLocationZ);  // 閉じる位置

	FVector currentLocation = GetOwner()->GetActorLocation();  // 現在の位置を取得
	speed = (targetLocation - currentLocation).Length() / moveTime;  // 移動に必要なスピードを計算

	FVector newLocation = FMath::VInterpConstantTo(currentLocation, targetLocation, deltaTime, speed);  // 定速で補間移動
	GetOwner()->SetActorLocation(newLocation);  // 新しい位置を設定して移動させる
}

// 5秒のタイマーを開始し、音を再生する処理
void UDoorMover::StartTimer() 
{
	isTimerActive = true;  // タイマーがアクティブであることを示すフラグをON

	GetWorld()->GetTimerManager().SetTimer(
		timerHandle, 				// タイマーハンドル（終了時の管理に使う）
		this, 						// 呼び出し先（このクラス）
		&UDoorMover::ResetTimer, 	// 終了時に呼び出す関数
		5.0f, 						// 時間（5秒後）
		false						// 繰り返さない（1回きり）
	);  // 5秒後にResetTimerを呼ぶタイマーを設定

	if(audioComponent && soundToPlay && !audioComponent->IsPlaying())  // 再生する音があり、現在再生していなければ
	{
		if(isTimerActive)  // タイマーが有効であれば
		{
			audioComponent->SetSound(soundToPlay);  // サウンドをセット
			audioComponent->Play();  // サウンドを再生
			UE_LOG(LogTemp, Warning, TEXT("Audio should now be playing!"));  // 再生開始ログ
		}
	}

	UE_LOG(LogTemp, Warning, TEXT("タイマー開始！ 5秒カウント中"));  // タイマー開始ログ
}

// タイマー終了時の処理
void UDoorMover::ResetTimer() 
{
	timerValue = 0.0f;  // タイマー値をリセット
	isTimerActive = false;  // タイマーフラグをOFFにする

	if(audioComponent && soundToPlay && audioComponent->IsPlaying())  // 音が再生中の場合
	{
		if(!isTimerActive)  // タイマーが無効なら音を停止
		{
			audioComponent->SetSound(soundToPlay);  // サウンドを再設定（再生前提）
			audioComponent->Stop();  // 再生を停止
			UE_LOG(LogTemp, Warning, TEXT("Audio should now be NoPplaying!"));  // 停止ログ（typo: "NoPplaying" → "Not Playing"かも）
		}
	}

	UE_LOG(LogTemp, Warning, TEXT("タイマーリセット！"));  // タイマー終了ログ
}
