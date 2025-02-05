// Fill out your copyright notice in the Description page of Project Settings.


#include "MovingFloor.h"

// Sets default values
AMovingFloor::AMovingFloor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("SceneRoot"));
	SetRootComponent(SceneRoot);

	StaticMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("staticMesh"));
	StaticMeshComp->SetupAttachment(SceneRoot);
	Speed = FMath::RandRange(100.0f, 500.0f);
	TimeRandom = FMath::RandRange(3.0f, 6.0f);
}

// Called when the game starts or when spawned
void AMovingFloor::BeginPlay()
{
	Super::BeginPlay();

	GetWorld()->GetTimerManager().SetTimer(
		TimerHandle,        // 타이머 핸들
		this,               // 함수 소유 객체
		&AMovingFloor::MyTimerFunction,  // 실행할 함수
		TimeRandom,               // 대기 시간(초)
		true              // 반복 여부
	);
	
}

// Called every frame
void AMovingFloor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!FMath::IsNearlyZero(Speed))
	{
		FTransform DeltaTransform;
		DeltaTransform.SetTranslation(FVector(DeltaTime * Speed, 0.0f, 0.0f));
		AddActorLocalTransform(DeltaTransform, false, nullptr);
	}
}



void AMovingFloor::MyTimerFunction()
{
	Speed = -Speed;
}