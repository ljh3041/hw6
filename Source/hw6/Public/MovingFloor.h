// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MovingFloor.generated.h"

UCLASS()
class HW6_API AMovingFloor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMovingFloor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "MovingFloor|Components")
	USceneComponent* SceneRoot;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MovingFloor|Components")
	UStaticMeshComponent* StaticMeshComp;
	float Speed;
	float TimeRandom;
	FTimerHandle TimerHandle;
	void MyTimerFunction();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
