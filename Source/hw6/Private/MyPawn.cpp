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

    // 1. 캡슐 컴포넌트 생성 (충돌 담당)
    CapsuleComponent = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CapsuleComponent"));
    RootComponent = CapsuleComponent; // 루트 컴포넌트로 설정

    // 2. 캐릭터 메쉬 추가
    MeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("MeshComponent"));
    MeshComponent->SetupAttachment(CapsuleComponent); // 캡슐에 부착

    // 3. Spring Arm (카메라 거리 조절)
    SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComponent"));
    SpringArmComponent->SetupAttachment(RootComponent);
    SpringArmComponent->TargetArmLength = 300.0f; // 카메라와 캐릭터 사이 거리 설정
    SpringArmComponent->bUsePawnControlRotation = false; // 마우스 입력에 따라 회전

    // 4. 카메라 추가
    CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComponent"));
    CameraComponent->SetupAttachment(SpringArmComponent, USpringArmComponent::SocketName); // 스프링암에 부착
    CameraComponent->bUsePawnControlRotation = false;

    bIsGrounded = true;
}

// Called when the game starts or when spawned
void AMyPawn::BeginPlay()
{
	Super::BeginPlay();
    // 물리 시뮬레이션 비활성화
    CapsuleComponent->SetSimulatePhysics(false);
    MeshComponent->SetSimulatePhysics(false);
	
}




// Called every frame
void AMyPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

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
void AMyPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

    // Enhanced InputComponent로 캐스팅
    if (UEnhancedInputComponent* EnhancedInput = Cast<UEnhancedInputComponent>(PlayerInputComponent))
    {
        // IA를 가져오기 위해 현재 소유 중인 Controller를 ASpartaPlayerController로 캐스팅
        if (AMyPlayerController* PlayerController = Cast<AMyPlayerController>(GetController()))
        {
            if (PlayerController->MoveAction)
            {
                // IA_Move 액션 키를 "키를 누르고 있는 동안" Move() 호출
                EnhancedInput->BindAction(PlayerController->MoveAction, ETriggerEvent::Triggered, this, &AMyPawn::MoveForward);
                EnhancedInput->BindAction(PlayerController->MoveAction, ETriggerEvent::Triggered, this, &AMyPawn::MoveRight);
            }

            if (PlayerController->LookAction)
            {
                // IA_Look 액션 마우스가 "움직일 때" Look() 호출
                EnhancedInput->BindAction(
                    PlayerController->LookAction,
                    ETriggerEvent::Triggered,
                    this,
                    &AMyPawn::Look
                );
            }

            if (PlayerController->JumpAction)
            {
                // IA_Move 액션 키를 "키를 누르고 있는 동안" Move() 호출
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
        float Speed = bIsGrounded ? 600.0f : 300.0f; // 공중에서는 속도를 줄임
        // 이동 방향 계산 (카메라의 방향을 기준으로 이동)
        FVector MoveDirection = FVector(1,0,0) * MoveInput.X;


        // 이동 적용
        AddActorLocalOffset(MoveDirection * Speed * GetWorld()->GetDeltaSeconds(), true);
    }
}



void AMyPawn::MoveRight(const FInputActionValue& Value)
{
    if (!Controller) return;

    const FVector2D MoveInput = Value.Get<FVector2D>();

    if (!FMath::IsNearlyZero(MoveInput.Y))
    {
        float Speed = bIsGrounded ? 600.0f : 300.0f; // 공중에서는 속도를 줄임
        // 이동 방향 계산 (카메라의 방향을 기준으로 이동)
        FVector MoveDirection = FVector(0, 1, 0) * MoveInput.Y;


        // 이동 적용
        AddActorLocalOffset(MoveDirection * Speed * GetWorld()->GetDeltaSeconds(), true);
    }
}



void AMyPawn::Look(const FInputActionValue& Value)
{
    // 마우스의 X, Y 움직임을 2D 축으로 가져옴
    FVector2D LookInput = Value.Get<FVector2D>();
    if (!Controller) return;

    // 1. 좌우(Yaw) 회전 -> Pawn 자체 회전
    AddActorLocalRotation(FRotator(0.0f, LookInput.X, 0.0f));

    // 2. 상하(Pitch) 회전 -> SpringArmComponent 회전
    FRotator SpringArmRotation = SpringArmComponent->GetRelativeRotation();
    float NewPitch = FMath::Clamp(SpringArmRotation.Pitch - LookInput.Y, -80.0f, 80.0f); // 상하 회전 제한
    SpringArmComponent->SetRelativeRotation(FRotator(NewPitch, 0.0f, 0.0f));
}


void AMyPawn::Jump(const FInputActionValue& value)
{
    if (value.Get<bool>())
    {
        if (bIsGrounded)
        {
            Velocity.Z = 600.0f; // 점프 높이 설정
            bIsGrounded = false;
        }
    }
}



// 바닥 감지 함수 (Capsule 아래 부분에서 LineTrace 사용)
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