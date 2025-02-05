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

    // ������ ������ Ŭ����
    UPROPERTY(EditAnywhere, Category = "Spawner Settings")
    TSubclassOf<AFloor> RotateFloorClass;

    // ������ ������ ����
    UPROPERTY(EditAnywhere, Category = "Spawner Settings")
    int32 NumberOfActorsToSpawn = 5;

    // ���� ���� ����
    UPROPERTY(EditAnywhere, Category = "Spawner Settings")
    FVector SpawnAreaMin = FVector(-100.f, -100.f, 10.f); // �ּ� ���� ��ǥ
    UPROPERTY(EditAnywhere, Category = "Spawner Settings")
    FVector SpawnAreaMax = FVector(100.f, 100.f, 20.f);  // �ִ� ���� ��ǥ

    // ���� ��ġ ���� �Լ�
    FVector GetRandomSpawnLocation() const;

    // ���� �Լ�
    void SpawnRotateFloors();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
