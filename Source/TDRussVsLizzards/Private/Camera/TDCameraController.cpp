// TD Russ Vs Lizzards Game

#include "Camera/TDCameraController.h"
#include "EnhancedInputComponent.h"
#include "InputActionValue.h"
#include "EnhancedInputSubsystems.h"
#include "DrawDebugHelpers.h"

DEFINE_LOG_CATEGORY_STATIC(LogCameraController, Display, Display);

ATDCameraController::ATDCameraController() {}

void ATDCameraController::BeginPlay()
{
    Super::BeginPlay();
    bShowMouseCursor       = true;
    bEnableClickEvents     = true;
    bEnableMouseOverEvents = true;
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
        EnhancedInputComponent->BindAction(
            MoveCameraRightAction, ETriggerEvent::Triggered, this, &ATDCameraController::MoveCameraRightLeft);
        EnhancedInputComponent->BindAction(MoveCameraLeftAction, ETriggerEvent::Triggered, this, &ATDCameraController::MoveCameraRightLeft);
        EnhancedInputComponent->BindAction(ZoomUpCameraAction, ETriggerEvent::Triggered, this, &ATDCameraController::ZoomUpAction);
        EnhancedInputComponent->BindAction(ZoomDownCameraAction, ETriggerEvent::Triggered, this, &ATDCameraController::ZoomUpAction);
        EnhancedInputComponent->BindAction(RotateRightCameraAction, ETriggerEvent::Triggered, this, &ATDCameraController::RotateCamera);
        EnhancedInputComponent->BindAction(RotateLeftCameraAction, ETriggerEvent::Triggered, this, &ATDCameraController::RotateCamera);

        EnhancedInputComponent->BindAction(SetHeroDestinationAction, ETriggerEvent::Started, this, &ATDCameraController::SetLeftClickChois);
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
    checkf(OnMoveCameraUpDown.ExecuteIfBound(Value.Get<float>()), TEXT(" OnMoveCameraUpDown Delegate is not bound "));
}

void ATDCameraController::MoveCameraRightLeft(const FInputActionValue& Value)
{
    checkf(OnMoveCameraRightLeft.ExecuteIfBound(Value.Get<float>()), TEXT(" OnMoveCameraRightLeft Delegate is not bound "));
}

void ATDCameraController::ZoomUpAction(const FInputActionValue& Value)
{
    checkf(OnZoomChanged.ExecuteIfBound(Value.Get<float>()), TEXT(" OnZoomChanged Delegate is not bound "));
}

void ATDCameraController::RotateCamera(const FInputActionValue& Value)
{
    checkf(OnRotateCamera.ExecuteIfBound(Value.Get<float>()), TEXT(" OnRotateCamera Delegate is not bound "));
}

void ATDCameraController::SetLeftClickChois()
{
    checkf(OnLeftMouseClickChois.ExecuteIfBound(GetLeftClickHit()), TEXT(" OnSetHeroDestination Delegate is not bound "));
}

FHitResult ATDCameraController::GetLeftClickHit()
{
    FHitResult Hit;
    bool bHitSuccessful = false;
    FVector MouseWorldLocation{};
    FVector MouseWorldDirection{};

    bHitSuccessful = DeprojectMousePositionToWorld(MouseWorldLocation, MouseWorldDirection);
    checkf(bHitSuccessful, TEXT(" Unable to determine value Mouse Click"));

    FVector TraceEnd = MouseWorldLocation + MouseWorldDirection * 50000;

    bHitSuccessful = GetWorld()->LineTraceSingleByChannel(Hit, MouseWorldLocation, TraceEnd, ECollisionChannel::ECC_Visibility);
    checkf(bHitSuccessful, TEXT(" No HIT"));

    return Hit;
}
