// Fill out your copyright notice in the Description page of Project Settings.


#include "DoorMover.h"
#include "Engine/World.h"
#include "TimerManager.h"
#include "Math/UnrealMathUtility.h"

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

	originalLocation = GetOwner()->GetActorLocation();
	openDoorOfset = FVector(originalLocation.X,originalLocation.Y,originalLocation.Z + openDoorTransformLocationZ);
	closeDoorOfset = FVector(originalLocation.X,originalLocation.Y,originalLocation.Z + closeDoorTransformLocationZ);
	if(audioComponent)
	{
		audioComponent->bAutoActivate = false;
	}
	time = GetWorld()->GetTimeSeconds();
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
	if (!audioComponent)
    {
        UE_LOG(LogTemp, Error, TEXT("AudioComponent is NULL!"));
    }
    if (!soundToPlay)
    {
        UE_LOG(LogTemp, Error, TEXT("SoundToPlay is NULL!"));
    }
	// ...
}
void UDoorMover::SetShouldMove(bool openClose)
{
	shouldMove = openClose;
}
void UDoorMover::StorneDoorMoverFunction(float deltaTime)
{
	targetLocation = shouldMove ? openDoorOfset = FVector(originalLocation.X,originalLocation.Y,openDoorTransformLocationZ) : closeDoorOfset = FVector(originalLocation.X,originalLocation.Y,closeDoorTransformLocationZ);
	FVector currentLocation = GetOwner()->GetActorLocation();
	speed = (targetLocation - currentLocation).Length() / moveTime;

	FVector newLocation = FMath::VInterpConstantTo(currentLocation,targetLocation,deltaTime,speed);
	GetOwner()->SetActorLocation(newLocation);
}
void UDoorMover::StartTimer()
{
	isTimerActive = true;
	GetWorld()->GetTimerManager().SetTimer(timerHandle,this,&UDoorMover::ResetTimer,5.0f,false);
	if(audioComponent && soundToPlay && !audioComponent->IsPlaying())
	{
		if(isTimerActive)
		{
			audioComponent->SetSound(soundToPlay);
			audioComponent->Play();
			UE_LOG(LogTemp, Warning, TEXT("Audio should now be playing!"));
		}
	}
	UE_LOG(LogTemp, Warning, TEXT("タイマー開始！ 5秒カウント中"));
}
void UDoorMover::ResetTimer()
{
	timerValue = 0.0f;
	isTimerActive = false;
	if(audioComponent && soundToPlay && audioComponent->IsPlaying())
	{
		if(!isTimerActive)
		{
			audioComponent->SetSound(soundToPlay);
			audioComponent->Stop();
			UE_LOG(LogTemp, Warning, TEXT("Audio should now be NoPplaying!"));
		}
	}
	UE_LOG(LogTemp, Warning, TEXT("タイマーリセット！"));
}
