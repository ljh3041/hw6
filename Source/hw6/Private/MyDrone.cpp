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

    // 1. 캡슐 컴포넌트 생성 (충돌 담당)
    CapsuleComponent = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CapsuleComponent"));
    RootComponent = CapsuleComponent; // 루트 컴포넌트로 설정

    // 2. 캐릭터 메쉬 추가
    StaticMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("staticMesh"));
    StaticMeshComp->SetupAttachment(RootComponent);
    // 3. Spring Arm (카메라 거리 조절)
    SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComponent"));
    SpringArmComponent->SetupAttachment(RootComponent);
    SpringArmComponent->TargetArmLength = 300.0f; // 카메라와 캐릭터 사이 거리 설정
    SpringArmComponent->bUsePawnControlRotation = false; // 마우스 입력에 따라 회전

    // 4. 카메라 추가
    CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComponent"));
    CameraComponent->SetupAttachment(SpringArmComponent, USpringArmComponent::SocketName); // 스프링암에 부착
    CameraComponent->bUsePawnControlRotation = false;
}

void AMyDrone::BeginPlay()
{
    Super::BeginPlay();
    // 물리 시뮬레이션 비활성화
    CapsuleComponent->SetSimulatePhysics(false);
    StaticMeshComp->SetSimulatePhysics(false);

}

// Called every frame
void AMyDrone::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

    // 현재 회전값 가져오기
    FRotator CurrentRotation = GetActorRotation();

    // 새로운 Roll 값 계산 (기울어지는 정도 조절 가능)
    float NewRoll = CurrentRotation.Roll - CurrentRotation.Roll * 0.005f; // 2.0f는 속도 조절 가능
    float NewPitch = CurrentRotation.Pitch - CurrentRotation.Pitch * 0.005f; // 2.0f는 속도 조절 가능
        
    // 새로운 회전값 적용
    SetActorRotation(FRotator(NewPitch, CurrentRotation.Yaw, NewRoll));

    


    // 중력 적용(지면에 닿아 있지 않다면)
    if (!bIsGrounded)
    {
        Velocity.Z += Gravity * DeltaTime; // 중력 적용
    }

    // 이동 적용
    FVector NewLocation = GetActorLocation() + Velocity * DeltaTime;

    // 지면 감지
    if (CheckGrounded())
    {
        bIsGrounded = true;
        Velocity.Z = 0; // 착지 시 속도 초기화
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

    // Enhanced InputComponent로 캐스팅
    if (UEnhancedInputComponent* EnhancedInput = Cast<UEnhancedInputComponent>(PlayerInputComponent))
    {
        // IA를 가져오기 위해 현재 소유 중인 Controller를 ASpartaPlayerController로 캐스팅
        if (AMyDroneController* DroneController = Cast<AMyDroneController>(GetController()))
        {
            if (DroneController->MoveAction)
            {
                // IA_Move 액션 키를 "키를 누르고 있는 동안" Move() 호출
                EnhancedInput->BindAction(DroneController->MoveAction, ETriggerEvent::Triggered, this, &AMyDrone::Move);
            }

            if (DroneController->LookAction)
            {
                // IA_Look 액션 마우스가 "움직일 때" Look() 호출
                EnhancedInput->BindAction(
                    DroneController->LookAction,
                    ETriggerEvent::Triggered,
                    this,
                    &AMyDrone::Look
                );
            }

            if (DroneController->UpDownAction)
            {
                // IA_Move 액션 키를 "키를 누르고 있는 동안" Move() 호출
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

        // 새로운 Roll 값 계산 (기울어지는 정도 조절 가능)
        float NewRoll = CurrentRotation.Roll + (MoveInput.Y * 0.1f); // 0.1f는 속도 조절 가능

        // Roll 값 제한 (너무 많이 기울어지지 않도록)
        NewRoll = FMath::Clamp(NewRoll, -10.0f, 10.0f); // -10도 ~ 10도 사이로 제한

        // 새로운 회전값 적용
        SetActorRotation(FRotator(CurrentRotation.Pitch, CurrentRotation.Yaw, NewRoll));

        // 이동 적용
        
        AddActorLocalOffset(MoveDirection * NewRoll * 100.0f * GetWorld()->GetDeltaSeconds(), true);
        
        
    }

    if (!FMath::IsNearlyZero(MoveInput.X))
    {
        // 이동 방향 계산 (카메라의 방향을 기준으로 이동)
        FVector MoveDirection = FVector(1, 0, 0);

        FRotator CurrentRotation = GetActorRotation();
        // 이동 적용
        // 새로운 Roll 값 계산 (기울어지는 정도 조절 가능)
        float NewPitch = CurrentRotation.Pitch - (MoveInput.X * 0.1f); // 2.0f는 속도 조절 가능

        // Roll 값 제한 (너무 많이 기울어지지 않도록)
        NewPitch = FMath::Clamp(NewPitch, -10.0f, 10.0f); // -30도 ~ 30도 사이로 제한

        // 새로운 회전값 적용
        SetActorRotation(FRotator(NewPitch, CurrentRotation.Yaw, CurrentRotation.Roll));

        // 이동 적용

        AddActorLocalOffset(-MoveDirection * NewPitch * 100.0f * GetWorld()->GetDeltaSeconds(), true);
    }
}


void AMyDrone::Look(const FInputActionValue& Value)
{
    // 마우스의 X, Y 움직임을 2D 축으로 가져옴
    FVector2D LookInput = Value.Get<FVector2D>();
    if (!Controller) return;

    // 1. 좌우(Yaw) 회전 -> Pawn 자체 회전
    AddActorLocalRotation(FRotator(0.0f, LookInput.X, 0.0f));

    // 2. 상하(Pitch) 회전 -> SpringArmComponent 회전
    FRotator SpringArmRotation = SpringArmComponent->GetRelativeRotation();
    float NewPitch = FMath::Clamp(SpringArmRotation.Pitch + LookInput.Y, -40.0f, 40.0f); // 상하 회전 제한
    SpringArmComponent->SetRelativeRotation(FRotator(NewPitch, 0.0f, 0.0f));
}


void AMyDrone::Rotate(const FInputActionValue& Value)
{
    const float RotateInput = Value.Get<float>();
    if (!Controller) return;

    if (!FMath::IsNearlyZero(RotateInput))
    {
        // 현재 회전값 가져오기
        FRotator CurrentRotation = GetActorRotation();

        // 새로운 Roll 값 계산 (기울어지는 정도 조절 가능)
        float NewRoll = CurrentRotation.Roll + (RotateInput * 0.1f); // 2.0f는 속도 조절 가능

        // Roll 값 제한 (너무 많이 기울어지지 않도록)
        NewRoll = FMath::Clamp(NewRoll, -10.0f, 10.0f); // -30도 ~ 30도 사이로 제한

        // 새로운 회전값 적용
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

// 바닥 감지 함수 (Capsule 아래 부분에서 LineTrace 사용)
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