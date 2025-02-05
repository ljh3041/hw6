// Fill out your copyright notice in the Description page of Project Settings.


#include "MyActorSpawner.h"
#include "Floor.h"// 스폰할 클래스 헤더 포함
#include "Kismet/KismetMathLibrary.h" // FMath::RandRange 또는 범위 계산용

// Sets default values
AMyActorSpawner::AMyActorSpawner()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AMyActorSpawner::BeginPlay()
{
	Super::BeginPlay();

	// 게임 시작 시 액터 스폰
	SpawnRotateFloors();
}

// Called every frame
void AMyActorSpawner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

FVector AMyActorSpawner::GetRandomSpawnLocation() const
{
    // X, Y, Z 값을 범위 내에서 랜덤으로 계산
    float RandomX = FMath::RandRange(SpawnAreaMin.X, SpawnAreaMax.X);
    float RandomY = FMath::RandRange(SpawnAreaMin.Y, SpawnAreaMax.Y);
    float RandomZ = FMath::RandRange(SpawnAreaMin.Z, SpawnAreaMax.Z);

    return FVector(RandomX, RandomY, RandomZ);
}

void AMyActorSpawner::SpawnRotateFloors()
{
    if (!RotateFloorClass) return; // RotateFloorClass가 유효하지 않으면 중단

    UWorld* World = GetWorld(); // 현재 월드 가져오기
    if (!World) return;

    for (int32 i = 0; i < NumberOfActorsToSpawn; i++)
    {
        FVector SpawnLocation = GetRandomSpawnLocation(); // 랜덤 위치 계산
        FRotator SpawnRotation = FRotator::ZeroRotator;   // 회전 각도 (필요 시 랜덤화 가능)

        // 액터 스폰
        AFloor* SpawnedActor = World->SpawnActor<AFloor>(
            RotateFloorClass,
            SpawnLocation,
            SpawnRotation
        );

        if (SpawnedActor)
        {
            UE_LOG(LogTemp, Log, TEXT("MyRotateFloor 스폰 성공: %s"), *SpawnedActor->GetName());
        }
        else
        {
            UE_LOG(LogTemp, Warning, TEXT("MyRotateFloor 스폰 실패!"));
        }
    }
}