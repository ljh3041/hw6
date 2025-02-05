// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "MyDrone.generated.h"

class USpringArmComponent;
class UCameraComponent;
struct FInputActionValue;



UCLASS()
class HW6_API AMyDrone : public APawn
{
    GENERATED_BODY()

public:
    // Sets default values for this pawn's properties
    AMyDrone();

    // Called every frame
    virtual void Tick(float DeltaTime) override;

    // Called to bind functionality to input
    virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;


protected:
    // Called when the game starts or when spawned
    virtual void BeginPlay() override;
    /** 충돌 캡슐 */
    UPROPERTY(VisibleAnywhere)
    class UCapsuleComponent* CapsuleComponent;

    /** 캐릭터 메쉬 */
    UPROPERTY(VisibleAnywhere)
    class UStaticMeshComponent* StaticMeshComp;

    /** 카메라 붐 (Spring Arm) */
    UPROPERTY(VisibleAnywhere)
    class USpringArmComponent* SpringArmComponent;

    /** 플레이어 카메라 */
    UPROPERTY(VisibleAnywhere)
    class UCameraComponent* CameraComponent;

    // IA_Move와 IA_Look 등을 처리할 함수 원형
    // Enhanced Input에서 액션 값은 FInputActionValue로 전달됩니다.
    UFUNCTION()
    void Move(const FInputActionValue& Value);
    UFUNCTION()
    void Rotate(const FInputActionValue& Value);
    UFUNCTION()
    void Look(const FInputActionValue& value);
    UFUNCTION()
    void UpDown(const FInputActionValue& value);

    bool CheckGrounded();

    FVector Velocity;
    float Gravity = -980.0f;   // Unreal의 기본 중력
    bool bIsGrounded = true;
    float AirControlFactor = 0.5f; // 공중 제어 비율

};