// TD Russ Vs Lizzards Game

#include "Camera/TDCameraController.h"
#include "EnhancedInputComponent.h"
#include "InputActionValue.h"
#include "EnhancedInputSubsystems.h"
#include "Camera/CameraPawn.h"
#include "Kismet/GameplayStatics.h"

DEFINE_LOG_CATEGORY_STATIC(LogCameraController, Display, Display);


ATDCameraController::ATDCameraController() {}

void ATDCameraController::BeginPlay()
{
    Super::BeginPlay();

    CameraPawn = Cast<ACameraPawn>(GetPawn());
}

void ATDCameraController::SetupInputComponent()
{
    Super::SetupInputComponent();

    // Add Input Mapping Context
    if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
    {
        Subsystem->AddMappingContext(DefaultMappingContext, 0);
    }

    // Set up action bindings
    if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(InputComponent))
    {
        EnhancedInputComponent->BindAction(MoveCameraUpAction, ETriggerEvent::Triggered, this, &ATDCameraController::MoveCameraUpDown);
        EnhancedInputComponent->BindAction(MoveCameraDownAction, ETriggerEvent::Triggered, this, &ATDCameraController::MoveCameraUpDown);
        EnhancedInputComponent->BindAction(MoveCameraRightAction, ETriggerEvent::Triggered, this, &ATDCameraController::MoveCameraRightLeft);
        EnhancedInputComponent->BindAction(MoveCameraLeftAction, ETriggerEvent::Triggered, this, &ATDCameraController::MoveCameraRightLeft);
        EnhancedInputComponent->BindAction(ZoomUpCameraAction, ETriggerEvent::Triggered, this, &ATDCameraController::ZoomUpAction);
        EnhancedInputComponent->BindAction(ZoomDownCameraAction, ETriggerEvent::Triggered, this, &ATDCameraController::ZoomUpAction);

    }
    else
    {
        UE_LOG(LogCameraController, Error,
            TEXT(
                "'%s' Failed to find an Enhanced Input Component! This template is built to use the Enhanced Input system. If you intend "),
            *GetNameSafe(this));
    }
}

void ATDCameraController::MoveCameraUpDown(const FInputActionValue& Value)
{

    if (CameraPawn)
    {
        float DeltaTime = UGameplayStatics::GetWorldDeltaSeconds(GetWorld());
        CameraPawn->SetActorLocation(
            CameraPawn->GetActorLocation() + CameraPawn->GetActorForwardVector() * Value.Get<float>() * SpeedCamera * DeltaTime);
    }
    else
    {
        UE_LOG(LogCameraController, Error, TEXT(" Camera pawn not initialize "));
        checkNoEntry();
    }
}



void ATDCameraController::MoveCameraRightLeft(const FInputActionValue& Value)
{

    if (CameraPawn)
    {
        float DeltaTime = UGameplayStatics::GetWorldDeltaSeconds(GetWorld());
        CameraPawn->SetActorLocation(
            CameraPawn->GetActorLocation() + CameraPawn->GetActorRightVector() * Value.Get<float>() * SpeedCamera * DeltaTime);
    }
    else
    {
        UE_LOG(LogCameraController, Error, TEXT(" Camera pawn not initialize "));
        checkNoEntry();
    }
}


void ATDCameraController::ZoomUpAction(const FInputActionValue& Value)
{
    OnZoomChanged.ExecuteIfBound(Value.Get<float>());
}

