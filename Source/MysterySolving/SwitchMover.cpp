// Fill out your copyright notice in the Description page of Project Settings.


#include "SwitchMover.h"
#include "Components/StaticMeshComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "DrawDebugHelpers.h"
#include "Engine/World.h"

// Sets default values for this component's properties
USwitchMover::USwitchMover()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void USwitchMover::BeginPlay()
{
	Super::BeginPlay();
	meshComponent = GetOwner()->FindComponentByClass<UStaticMeshComponent>(); //メッシュ取得
	if(meshComponent)
	{
		meshComponent->SetSimulatePhysics(standPushJudge); //初期の物理無効
		meshComponent->SetCollisionResponseToChannel(ECC_Pawn,ECR_Overlap); //初期はPwanの当たり判定はOverlap 
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("MeshComponent not found:")); //メッシュがない時エラー表示
	}
	// ...
	
}


// Called every frame
void USwitchMover::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	meshComponent->SetSimulatePhysics(standPushJudge); 
	if (standPushJudge && meshComponent)
	{
		FVector ownerLoc = GetOwner()->GetActorLocation(); // 4方向の障害物判定
		// 4方向それぞれで2m以内の障害物を判定
		bool bNearObstacleRight = IsObstacleNearby(FVector(200.0f, 0.0f, 0.0f));   // +X (右)
		bool bNearObstacleLeft = IsObstacleNearby(FVector(-200.0f, 0.0f, 0.0f));  // -X (左)
		bool bNearObstacleForward = IsObstacleNearby(FVector(0.0f, 200.0f, 0.0f)); // +Y (前)
		bool bNearObstacleBackward = IsObstacleNearby(FVector(0.0f, -200.0f, 0.0f)); // -Y (後)
		// デバッグ表示
		DrawDebugLine(GetWorld(), ownerLoc, ownerLoc + FVector(200.0f, 0.0f, 0.0f), 
			bNearObstacleRight ? FColor::Red : FColor::Green, false, 0.1f, 0, 2.0f);    // 右
		DrawDebugLine(GetWorld(), ownerLoc, ownerLoc + FVector(-200.0f, 0.0f, 0.0f), 
			bNearObstacleLeft ? FColor::Red : FColor::Green, false, 0.1f, 0, 2.0f);     // 左
		DrawDebugLine(GetWorld(), ownerLoc, ownerLoc + FVector(0.0f, 200.0f, 0.0f), 
			bNearObstacleForward ? FColor::Red : FColor::Green, false, 0.1f, 0, 2.0f);  // 前
		DrawDebugLine(GetWorld(), ownerLoc, ownerLoc + FVector(0.0f, -200.0f, 0.0f), 
			bNearObstacleBackward ? FColor::Red : FColor::Green, false, 0.1f, 0, 2.0f); // 後

		// 2m以内は無視する仕組み
		bool bCanMove = true;

		// X方向
		if (bNearObstacleRight || bNearObstacleLeft)
		{
			FHitResult hitRight, hitLeft;
			FCollisionQueryParams params;
			params.AddIgnoredActor(GetOwner());

			bool bHitRight = GetWorld()->LineTraceSingleByChannel(
				hitRight, ownerLoc, ownerLoc + FVector(200.0f, 0.0f, 0.0f), ECC_Visibility, params);

			bool bHitLeft = GetWorld()->LineTraceSingleByChannel(
				hitLeft, ownerLoc, ownerLoc + FVector(-200.0f, 0.0f, 0.0f), ECC_Visibility, params);

			// 2mより遠い障害物なら停止
			if ((bHitRight && hitRight.Distance > 200.0f) || (bHitLeft && hitLeft.Distance > 200.0f))
			{
				bCanMove = false;
			}
		}

		// Y方向
		if (bNearObstacleForward || bNearObstacleBackward)
		{
			FHitResult hitForward, hitBackward;
			FCollisionQueryParams params;

			bool bHitForward = GetWorld()->LineTraceSingleByChannel(
				hitForward, ownerLoc, ownerLoc + FVector(0.0f, 200.0f, 0.0f), ECC_Visibility, params);

			bool bHitBackward = GetWorld()->LineTraceSingleByChannel(
				hitBackward, ownerLoc, ownerLoc + FVector(0.0f, -200.0f, 0.0f), ECC_Visibility, params);

			// 2mより遠い障害物なら停止
			if ((bHitForward && hitForward.Distance > 200.0f) || (bHitBackward && hitBackward.Distance > 200.0f))
			{
				bCanMove = false;
			}
		}

		meshComponent->SetCollisionResponseToChannel(ECC_Pawn, ECR_Block);
	}
	else if (meshComponent)
	{
		meshComponent->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	}
}
// プレイヤーがぶつかった時に呼ばれる
void USwitchMover::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	// 押す方向を前後左右に制限
	FVector Direction = -Hit.ImpactNormal;
	if (FMath::Abs(Direction.X) > FMath::Abs(Direction.Y))
	{
		pushDirection = FVector(FMath::Sign(Direction.X), 0.0f, 0.0f);
	}
	else
	{
		pushDirection = FVector(0.0f, FMath::Sign(Direction.Y), 0.0f);
	}
}

// 床以外との距離が2m以内か判定
bool USwitchMover::IsObstacleNearby(const FVector& Offset)
{
	FVector start = GetOwner()->GetActorLocation();
	FVector end = start + Offset;  // 判定方向を指定

	FCollisionQueryParams traceParams;
	traceParams.AddIgnoredActor(GetOwner());

	TArray<FHitResult> hitResults;
	GetWorld()->SweepMultiByChannel(
		hitResults,
		start,
		end,
		FQuat::Identity,
		ECC_Visibility,
		FCollisionShape::MakeSphere(50.0f),  // 衝突判定サイズ
		traceParams
	);

	for (const FHitResult& hit : hitResults)
	{
		if (hit.GetActor() && !hit.GetActor()->GetName().Contains(TEXT("Floor")))
		{
			return true;  // 床以外のアクターに衝突
		}
	}
	return false;  // 障害物なし
}

void USwitchMover::SetStandPushJudge(bool isPushing)
{
	standPushJudge = isPushing;
}