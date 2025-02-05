#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "MyDroneController.generated.h"


class UInputMappingContext;
class UInputAction;


UCLASS()
class HW6_API AMyDroneController : public APlayerController
{
	GENERATED_BODY()


public:

	AMyDroneController();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	UInputMappingContext* InputMappingContext;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	UInputAction* MoveAction;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	UInputAction* LookAction;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	UInputAction* RotateAction;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	UInputAction* UpDownAction;

	virtual void BeginPlay() override;
	
};
