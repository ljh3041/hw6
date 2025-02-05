// Fill out your copyright notice in the Description page of Project Settings.


#include "RotateFloor.h"

// Sets default values
ARotateFloor::ARotateFloor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("SceneRoot"));
	SetRootComponent(SceneRoot);

	StaticMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("staticMesh"));
	StaticMeshComp->SetupAttachment(SceneRoot);
	RotationSpeed = FMath::RandRange(45.0f, 90.0f);
}

// Called when the game starts or when spawned
void ARotateFloor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ARotateFloor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (!FMath::IsNearlyZero(RotationSpeed))
	{
		// 초당 RotationSpeed만큼, 한 프레임당 (RotationSpeed * DeltaTime)만큼 회전
		AddActorLocalRotation(FRotator(0.0f, RotationSpeed * DeltaTime, 0.0f));
	}
}

