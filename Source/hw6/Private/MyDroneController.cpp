// Fill out your copyright notice in the Description page of Project Settings.


#include "MyDroneController.h"
#include "EnhancedInputSubsystems.h"


AMyDroneController::AMyDroneController() : 
	InputMappingContext(nullptr),
	MoveAction(nullptr),
	LookAction(nullptr),
	RotateAction(nullptr),
	UpDownAction(nullptr)
{

}


void AMyDroneController::BeginPlay()
{
	Super::BeginPlay();

	if (ULocalPlayer* LocalPlayer = GetLocalPlayer())
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem =
			LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>())
		{
			if (InputMappingContext)
			{
				Subsystem->AddMappingContext(InputMappingContext, 0);
			}
		}
	}

}