// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "MyPawn.generated.h"

class USpringArmComponent;
class UCameraComponent;
struct FInputActionValue;


UCLASS()
class HW6_API AMyPawn : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	AMyPawn();
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
    class USkeletalMeshComponent* MeshComponent;

    /** ī�޶� �� (Spring Arm) */
    UPROPERTY(VisibleAnywhere)
    class USpringArmComponent* SpringArmComponent;

    /** �÷��̾� ī�޶� */
    UPROPERTY(VisibleAnywhere)
    class UCameraComponent* CameraComponent;

    // IA_Move�� IA_Look ���� ó���� �Լ� ����
    // Enhanced Input���� �׼� ���� FInputActionValue�� ���޵˴ϴ�.
    UFUNCTION()
    void MoveForward(const FInputActionValue& Value);
    UFUNCTION()
    void MoveRight(const FInputActionValue& Value);
    UFUNCTION()
    void Look(const FInputActionValue& value);
    UFUNCTION()
    void Jump(const FInputActionValue& value);


    FVector Velocity;
    float Gravity = -980.0f;   // Unreal�� �⺻ �߷�
    bool bIsGrounded = true;
    float AirControlFactor = 0.5f; // ���� ���� ����

    bool CheckGrounded();



};
