#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Floor.h"
#include "MyActorSpawner.generated.h"



class AFloor;


UCLASS()
class HW6_API AMyActorSpawner : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMyActorSpawner();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

    // 스폰할 액터의 클래스
    UPROPERTY(EditAnywhere, Category = "Spawner Settings")
    TSubclassOf<AFloor> RotateFloorClass;

    // 스폰할 액터의 개수
    UPROPERTY(EditAnywhere, Category = "Spawner Settings")
    int32 NumberOfActorsToSpawn = 5;

    // 스폰 영역 범위
    UPROPERTY(EditAnywhere, Category = "Spawner Settings")
    FVector SpawnAreaMin = FVector(-100.f, -100.f, 10.f); // 최소 스폰 좌표
    UPROPERTY(EditAnywhere, Category = "Spawner Settings")
    FVector SpawnAreaMax = FVector(100.f, 100.f, 20.f);  // 최대 스폰 좌표

    // 랜덤 위치 생성 함수
    FVector GetRandomSpawnLocation() const;

    // 스폰 함수
    void SpawnRotateFloors();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
