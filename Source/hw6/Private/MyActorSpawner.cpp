// Fill out your copyright notice in the Description page of Project Settings.


#include "MyActorSpawner.h"
#include "Floor.h"// ������ Ŭ���� ��� ����
#include "Kismet/KismetMathLibrary.h" // FMath::RandRange �Ǵ� ���� ����

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

	// ���� ���� �� ���� ����
	SpawnRotateFloors();
}

// Called every frame
void AMyActorSpawner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

FVector AMyActorSpawner::GetRandomSpawnLocation() const
{
    // X, Y, Z ���� ���� ������ �������� ���
    float RandomX = FMath::RandRange(SpawnAreaMin.X, SpawnAreaMax.X);
    float RandomY = FMath::RandRange(SpawnAreaMin.Y, SpawnAreaMax.Y);
    float RandomZ = FMath::RandRange(SpawnAreaMin.Z, SpawnAreaMax.Z);

    return FVector(RandomX, RandomY, RandomZ);
}

void AMyActorSpawner::SpawnRotateFloors()
{
    if (!RotateFloorClass) return; // RotateFloorClass�� ��ȿ���� ������ �ߴ�

    UWorld* World = GetWorld(); // ���� ���� ��������
    if (!World) return;

    for (int32 i = 0; i < NumberOfActorsToSpawn; i++)
    {
        FVector SpawnLocation = GetRandomSpawnLocation(); // ���� ��ġ ���
        FRotator SpawnRotation = FRotator::ZeroRotator;   // ȸ�� ���� (�ʿ� �� ����ȭ ����)

        // ���� ����
        AFloor* SpawnedActor = World->SpawnActor<AFloor>(
            RotateFloorClass,
            SpawnLocation,
            SpawnRotation
        );

        if (SpawnedActor)
        {
            UE_LOG(LogTemp, Log, TEXT("MyRotateFloor ���� ����: %s"), *SpawnedActor->GetName());
        }
        else
        {
            UE_LOG(LogTemp, Warning, TEXT("MyRotateFloor ���� ����!"));
        }
    }
}