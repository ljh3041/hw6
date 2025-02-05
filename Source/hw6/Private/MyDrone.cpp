// Fill out your copyright notice in the Description page of Project Settings.


#include "MyDrone.h"
#include "MyDroneController.h"
#include "EnhancedInputComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/SkeletalMeshComponent.h"



AMyDrone::AMyDrone()
{

    PrimaryActorTick.bCanEverTick = true;

    // 1. ĸ�� ������Ʈ ���� (�浹 ���)
    CapsuleComponent = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CapsuleComponent"));
    RootComponent = CapsuleComponent; // ��Ʈ ������Ʈ�� ����

    // 2. ĳ���� �޽� �߰�
    StaticMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("staticMesh"));
    StaticMeshComp->SetupAttachment(RootComponent);
    // 3. Spring Arm (ī�޶� �Ÿ� ����)
    SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComponent"));
    SpringArmComponent->SetupAttachment(RootComponent);
    SpringArmComponent->TargetArmLength = 300.0f; // ī�޶�� ĳ���� ���� �Ÿ� ����
    SpringArmComponent->bUsePawnControlRotation = false; // ���콺 �Է¿� ���� ȸ��

    // 4. ī�޶� �߰�
    CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComponent"));
    CameraComponent->SetupAttachment(SpringArmComponent, USpringArmComponent::SocketName); // �������Ͽ� ����
    CameraComponent->bUsePawnControlRotation = false;
}

void AMyDrone::BeginPlay()
{
    Super::BeginPlay();
    // ���� �ùķ��̼� ��Ȱ��ȭ
    CapsuleComponent->SetSimulatePhysics(false);
    StaticMeshComp->SetSimulatePhysics(false);

}

// Called every frame
void AMyDrone::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

    // ���� ȸ���� ��������
    FRotator CurrentRotation = GetActorRotation();

    // ���ο� Roll �� ��� (�������� ���� ���� ����)
    float NewRoll = CurrentRotation.Roll - CurrentRotation.Roll * 0.005f; // 2.0f�� �ӵ� ���� ����
    float NewPitch = CurrentRotation.Pitch - CurrentRotation.Pitch * 0.005f; // 2.0f�� �ӵ� ���� ����
        
    // ���ο� ȸ���� ����
    SetActorRotation(FRotator(NewPitch, CurrentRotation.Yaw, NewRoll));

    


    // �߷� ����(���鿡 ��� ���� �ʴٸ�)
    if (!bIsGrounded)
    {
        Velocity.Z += Gravity * DeltaTime; // �߷� ����
    }

    // �̵� ����
    FVector NewLocation = GetActorLocation() + Velocity * DeltaTime;

    // ���� ����
    if (CheckGrounded())
    {
        bIsGrounded = true;
        Velocity.Z = 0; // ���� �� �ӵ� �ʱ�ȭ
    }
    else
    {
        bIsGrounded = false;
    }

    SetActorLocation(NewLocation, true);

}



// Called to bind functionality to input
void AMyDrone::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);

    // Enhanced InputComponent�� ĳ����
    if (UEnhancedInputComponent* EnhancedInput = Cast<UEnhancedInputComponent>(PlayerInputComponent))
    {
        // IA�� �������� ���� ���� ���� ���� Controller�� ASpartaPlayerController�� ĳ����
        if (AMyDroneController* DroneController = Cast<AMyDroneController>(GetController()))
        {
            if (DroneController->MoveAction)
            {
                // IA_Move �׼� Ű�� "Ű�� ������ �ִ� ����" Move() ȣ��
                EnhancedInput->BindAction(DroneController->MoveAction, ETriggerEvent::Triggered, this, &AMyDrone::Move);
            }

            if (DroneController->LookAction)
            {
                // IA_Look �׼� ���콺�� "������ ��" Look() ȣ��
                EnhancedInput->BindAction(
                    DroneController->LookAction,
                    ETriggerEvent::Triggered,
                    this,
                    &AMyDrone::Look
                );
            }

            if (DroneController->UpDownAction)
            {
                // IA_Move �׼� Ű�� "Ű�� ������ �ִ� ����" Move() ȣ��
                EnhancedInput->BindAction(DroneController->UpDownAction, ETriggerEvent::Triggered, this, &AMyDrone::UpDown);
            }

            if (DroneController->RotateAction)
            {
                EnhancedInput->BindAction(
                    DroneController->RotateAction,
                    ETriggerEvent::Triggered,
                    this,
                    &AMyDrone::Rotate
                );
            }

        }
    }
}


void AMyDrone::Move(const FInputActionValue& Value)
{
    if (!Controller) return;
    Velocity.Z = 0;
    const FVector2D MoveInput = Value.Get<FVector2D>();

    if (!FMath::IsNearlyZero(MoveInput.Y))
    {
        FVector MoveDirection = FVector(0, 1, 0);
        
        FRotator CurrentRotation = GetActorRotation();

        // ���ο� Roll �� ��� (�������� ���� ���� ����)
        float NewRoll = CurrentRotation.Roll + (MoveInput.Y * 0.1f); // 0.1f�� �ӵ� ���� ����

        // Roll �� ���� (�ʹ� ���� �������� �ʵ���)
        NewRoll = FMath::Clamp(NewRoll, -10.0f, 10.0f); // -10�� ~ 10�� ���̷� ����

        // ���ο� ȸ���� ����
        SetActorRotation(FRotator(CurrentRotation.Pitch, CurrentRotation.Yaw, NewRoll));

        // �̵� ����
        
        AddActorLocalOffset(MoveDirection * NewRoll * 100.0f * GetWorld()->GetDeltaSeconds(), true);
        
        
    }

    if (!FMath::IsNearlyZero(MoveInput.X))
    {
        // �̵� ���� ��� (ī�޶��� ������ �������� �̵�)
        FVector MoveDirection = FVector(1, 0, 0);

        FRotator CurrentRotation = GetActorRotation();
        // �̵� ����
        // ���ο� Roll �� ��� (�������� ���� ���� ����)
        float NewPitch = CurrentRotation.Pitch - (MoveInput.X * 0.1f); // 2.0f�� �ӵ� ���� ����

        // Roll �� ���� (�ʹ� ���� �������� �ʵ���)
        NewPitch = FMath::Clamp(NewPitch, -10.0f, 10.0f); // -30�� ~ 30�� ���̷� ����

        // ���ο� ȸ���� ����
        SetActorRotation(FRotator(NewPitch, CurrentRotation.Yaw, CurrentRotation.Roll));

        // �̵� ����

        AddActorLocalOffset(-MoveDirection * NewPitch * 100.0f * GetWorld()->GetDeltaSeconds(), true);
    }
}


void AMyDrone::Look(const FInputActionValue& Value)
{
    // ���콺�� X, Y �������� 2D ������ ������
    FVector2D LookInput = Value.Get<FVector2D>();
    if (!Controller) return;

    // 1. �¿�(Yaw) ȸ�� -> Pawn ��ü ȸ��
    AddActorLocalRotation(FRotator(0.0f, LookInput.X, 0.0f));

    // 2. ����(Pitch) ȸ�� -> SpringArmComponent ȸ��
    FRotator SpringArmRotation = SpringArmComponent->GetRelativeRotation();
    float NewPitch = FMath::Clamp(SpringArmRotation.Pitch + LookInput.Y, -40.0f, 40.0f); // ���� ȸ�� ����
    SpringArmComponent->SetRelativeRotation(FRotator(NewPitch, 0.0f, 0.0f));
}


void AMyDrone::Rotate(const FInputActionValue& Value)
{
    const float RotateInput = Value.Get<float>();
    if (!Controller) return;

    if (!FMath::IsNearlyZero(RotateInput))
    {
        // ���� ȸ���� ��������
        FRotator CurrentRotation = GetActorRotation();

        // ���ο� Roll �� ��� (�������� ���� ���� ����)
        float NewRoll = CurrentRotation.Roll + (RotateInput * 0.1f); // 2.0f�� �ӵ� ���� ����

        // Roll �� ���� (�ʹ� ���� �������� �ʵ���)
        NewRoll = FMath::Clamp(NewRoll, -10.0f, 10.0f); // -30�� ~ 30�� ���̷� ����

        // ���ο� ȸ���� ����
        SetActorRotation(FRotator(CurrentRotation.Pitch, CurrentRotation.Yaw, NewRoll));

    }
}

void AMyDrone::UpDown(const FInputActionValue& Value)
{
    Velocity.Z = 0;
    const float UpDownInput = Value.Get<float>();
    if (!Controller) return;

    FVector MoveDirection = FVector(0, 0, 1) * UpDownInput;

    AddActorLocalOffset(MoveDirection * 300.0f * GetWorld()->GetDeltaSeconds(), true);
}

// �ٴ� ���� �Լ� (Capsule �Ʒ� �κп��� LineTrace ���)
bool AMyDrone::CheckGrounded()
{

    FVector Start = GetActorLocation();
    FVector End = Start + FVector(0, 0, -CapsuleComponent->GetScaledCapsuleHalfHeight() - 10.0f);

    FHitResult HitResult;
    FCollisionQueryParams Params;
    Params.AddIgnoredActor(this);

    bool bHit = GetWorld()->LineTraceSingleByChannel(HitResult, Start, End, ECC_Visibility, Params);
    return bHit;
}