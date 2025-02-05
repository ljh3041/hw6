// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RotateFloor.generated.h"

UCLASS()
class HW6_API ARotateFloor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ARotateFloor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "RotateFloor|Components")
	USceneComponent* SceneRoot;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RotateFloor|Components")
	UStaticMeshComponent* StaticMeshComp;
	float RotationSpeed;
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
