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
    /** �浹 ĸ�� */
    UPROPERTY(VisibleAnywhere)
    class UCapsuleComponent* CapsuleComponent;

    /** ĳ���� �޽� */
    UPROPERTY(VisibleAnywhere)
    class UStaticMeshComponent* StaticMeshComp;

    /** ī�޶� �� (Spring Arm) */
    UPROPERTY(VisibleAnywhere)
    class USpringArmComponent* SpringArmComponent;

    /** �÷��̾� ī�޶� */
    UPROPERTY(VisibleAnywhere)
    class UCameraComponent* CameraComponent;

    // IA_Move�� IA_Look ���� ó���� �Լ� ����
    // Enhanced Input���� �׼� ���� FInputActionValue�� ���޵˴ϴ�.
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
    float Gravity = -980.0f;   // Unreal�� �⺻ �߷�
    bool bIsGrounded = true;
    float AirControlFactor = 0.5f; // ���� ���� ����

};