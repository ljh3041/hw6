// Fill out your copyright notice in the Description page of Project Settings.


#include "MyPawn.h"
#include "MyPlayerController.h"
#include "EnhancedInputComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/SkeletalMeshComponent.h"


// Sets default values
AMyPawn::AMyPawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

    // 1. ĸ�� ������Ʈ ���� (�浹 ���)
    CapsuleComponent = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CapsuleComponent"));
    RootComponent = CapsuleComponent; // ��Ʈ ������Ʈ�� ����

    // 2. ĳ���� �޽� �߰�
    MeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("MeshComponent"));
    MeshComponent->SetupAttachment(CapsuleComponent); // ĸ���� ����

    // 3. Spring Arm (ī�޶� �Ÿ� ����)
    SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComponent"));
    SpringArmComponent->SetupAttachment(RootComponent);
    SpringArmComponent->TargetArmLength = 300.0f; // ī�޶�� ĳ���� ���� �Ÿ� ����
    SpringArmComponent->bUsePawnControlRotation = false; // ���콺 �Է¿� ���� ȸ��

    // 4. ī�޶� �߰�
    CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComponent"));
    CameraComponent->SetupAttachment(SpringArmComponent, USpringArmComponent::SocketName); // �������Ͽ� ����
    CameraComponent->bUsePawnControlRotation = false;

    bIsGrounded = true;
}

// Called when the game starts or when spawned
void AMyPawn::BeginPlay()
{
	Super::BeginPlay();
    // ���� �ùķ��̼� ��Ȱ��ȭ
    CapsuleComponent->SetSimulatePhysics(false);
    MeshComponent->SetSimulatePhysics(false);
	
}




// Called every frame
void AMyPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

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
void AMyPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

    // Enhanced InputComponent�� ĳ����
    if (UEnhancedInputComponent* EnhancedInput = Cast<UEnhancedInputComponent>(PlayerInputComponent))
    {
        // IA�� �������� ���� ���� ���� ���� Controller�� ASpartaPlayerController�� ĳ����
        if (AMyPlayerController* PlayerController = Cast<AMyPlayerController>(GetController()))
        {
            if (PlayerController->MoveAction)
            {
                // IA_Move �׼� Ű�� "Ű�� ������ �ִ� ����" Move() ȣ��
                EnhancedInput->BindAction(PlayerController->MoveAction, ETriggerEvent::Triggered, this, &AMyPawn::MoveForward);
                EnhancedInput->BindAction(PlayerController->MoveAction, ETriggerEvent::Triggered, this, &AMyPawn::MoveRight);
            }

            if (PlayerController->LookAction)
            {
                // IA_Look �׼� ���콺�� "������ ��" Look() ȣ��
                EnhancedInput->BindAction(
                    PlayerController->LookAction,
                    ETriggerEvent::Triggered,
                    this,
                    &AMyPawn::Look
                );
            }

            if (PlayerController->JumpAction)
            {
                // IA_Move �׼� Ű�� "Ű�� ������ �ִ� ����" Move() ȣ��
                EnhancedInput->BindAction(PlayerController->JumpAction, ETriggerEvent::Triggered, this, &AMyPawn::Jump);
            }

        }
    }
}

void AMyPawn::MoveForward(const FInputActionValue& Value)
{
    if (!Controller) return;

    const FVector2D MoveInput = Value.Get<FVector2D>();

    if (!FMath::IsNearlyZero(MoveInput.X))
    {
        float Speed = bIsGrounded ? 600.0f : 300.0f; // ���߿����� �ӵ��� ����
        // �̵� ���� ��� (ī�޶��� ������ �������� �̵�)
        FVector MoveDirection = FVector(1,0,0) * MoveInput.X;


        // �̵� ����
        AddActorLocalOffset(MoveDirection * Speed * GetWorld()->GetDeltaSeconds(), true);
    }
}



void AMyPawn::MoveRight(const FInputActionValue& Value)
{
    if (!Controller) return;

    const FVector2D MoveInput = Value.Get<FVector2D>();

    if (!FMath::IsNearlyZero(MoveInput.Y))
    {
        float Speed = bIsGrounded ? 600.0f : 300.0f; // ���߿����� �ӵ��� ����
        // �̵� ���� ��� (ī�޶��� ������ �������� �̵�)
        FVector MoveDirection = FVector(0, 1, 0) * MoveInput.Y;


        // �̵� ����
        AddActorLocalOffset(MoveDirection * Speed * GetWorld()->GetDeltaSeconds(), true);
    }
}



void AMyPawn::Look(const FInputActionValue& Value)
{
    // ���콺�� X, Y �������� 2D ������ ������
    FVector2D LookInput = Value.Get<FVector2D>();
    if (!Controller) return;

    // 1. �¿�(Yaw) ȸ�� -> Pawn ��ü ȸ��
    AddActorLocalRotation(FRotator(0.0f, LookInput.X, 0.0f));

    // 2. ����(Pitch) ȸ�� -> SpringArmComponent ȸ��
    FRotator SpringArmRotation = SpringArmComponent->GetRelativeRotation();
    float NewPitch = FMath::Clamp(SpringArmRotation.Pitch - LookInput.Y, -80.0f, 80.0f); // ���� ȸ�� ����
    SpringArmComponent->SetRelativeRotation(FRotator(NewPitch, 0.0f, 0.0f));
}


void AMyPawn::Jump(const FInputActionValue& value)
{
    if (value.Get<bool>())
    {
        if (bIsGrounded)
        {
            Velocity.Z = 600.0f; // ���� ���� ����
            bIsGrounded = false;
        }
    }
}



// �ٴ� ���� �Լ� (Capsule �Ʒ� �κп��� LineTrace ���)
bool AMyPawn::CheckGrounded()
{
    FVector Start = GetActorLocation();
    FVector End = Start + FVector(0, 0, -CapsuleComponent->GetScaledCapsuleHalfHeight() - 10.0f);

    FHitResult HitResult;
    FCollisionQueryParams Params;
    Params.AddIgnoredActor(this);

    bool bHit = GetWorld()->LineTraceSingleByChannel(HitResult, Start, End, ECC_Visibility, Params);
    return bHit;
}