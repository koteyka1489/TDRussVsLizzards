// TD Russ Vs Lizzards Game

#include "Camera/TDCameraController.h"
#include "EnhancedInputComponent.h"
#include "InputActionValue.h"
#include "EnhancedInputSubsystems.h"
#include "DrawDebugHelpers.h"

DEFINE_LOG_CATEGORY_STATIC(LogCameraController, All, All);

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
    UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());
    if (!IsValid(Subsystem)) return;

    Subsystem->AddMappingContext(DefaultMappingContext, 1);

    UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(InputComponent);

    if (!IsValid(EnhancedInputComponent)) return;

    EnhancedInputComponent->BindAction(MoveCameraUpAction, ETriggerEvent::Triggered, this, &ATDCameraController::MoveCameraUpDown);
    EnhancedInputComponent->BindAction(MoveCameraDownAction, ETriggerEvent::Triggered, this, &ATDCameraController::MoveCameraUpDown);
    EnhancedInputComponent->BindAction(MoveCameraRightAction, ETriggerEvent::Triggered, this, &ATDCameraController::MoveCameraRightLeft);
    EnhancedInputComponent->BindAction(MoveCameraLeftAction, ETriggerEvent::Triggered, this, &ATDCameraController::MoveCameraRightLeft);
    EnhancedInputComponent->BindAction(ZoomUpCameraAction, ETriggerEvent::Triggered, this, &ATDCameraController::ZoomUpAction);
    EnhancedInputComponent->BindAction(ZoomDownCameraAction, ETriggerEvent::Triggered, this, &ATDCameraController::ZoomUpAction);
    EnhancedInputComponent->BindAction(RotateRightCameraAction, ETriggerEvent::Triggered, this, &ATDCameraController::RotateCamera);
    EnhancedInputComponent->BindAction(RotateLeftCameraAction, ETriggerEvent::Triggered, this, &ATDCameraController::RotateCamera);

    EnhancedInputComponent->BindAction(LeftClickMouse, ETriggerEvent::Started, this, &ATDCameraController::LeftClickStarted);
    EnhancedInputComponent->BindAction(LeftClickMouse, ETriggerEvent::Triggered, this, &ATDCameraController::LeftClickTriggered);
    EnhancedInputComponent->BindAction(LeftClickMouse, ETriggerEvent::Completed, this, &ATDCameraController::LeftClickCompleted);

    EnhancedInputComponent->BindAction(RightClickMouse, ETriggerEvent::Triggered, this, &ATDCameraController::RightClickTriggered);
    EnhancedInputComponent->BindAction(RightClickMouse, ETriggerEvent::Completed, this, &ATDCameraController::RightClickCompleted);

    EnhancedInputComponent->BindAction(CameraAngleUpAction, ETriggerEvent::Triggered, this, &ATDCameraController::ChangeAngleCamera);
    EnhancedInputComponent->BindAction(CameraAngleDownAction, ETriggerEvent::Triggered, this, &ATDCameraController::ChangeAngleCamera);

    EnhancedInputComponent->BindAction(
        MultiplySelectSquadsAction, ETriggerEvent::Started, this, &ATDCameraController::MultiplySelectSquadsOn);
    EnhancedInputComponent->BindAction(
        MultiplySelectSquadsAction, ETriggerEvent::Completed, this, &ATDCameraController::MultiplySelectSquadsOff);

    EnhancedInputComponent->BindAction(StopSquadAction, ETriggerEvent::Started, this, &ATDCameraController::StopSquad);

    EnhancedInputComponent->BindAction(GroupingSquadAction, ETriggerEvent::Completed, this, &ATDCameraController::GroupingSquad);
}

void ATDCameraController::MoveCameraUpDown(const FInputActionValue& Value)
{
    if (!OnMoveCameraUpDown.ExecuteIfBound(Value.Get<float>()))
    {
        UE_LOG(LogCameraController, Error, TEXT("OnMoveCameraUpDown Delegate is not bound"));
        checkNoEntry();
    }
}

void ATDCameraController::MoveCameraRightLeft(const FInputActionValue& Value)
{
    if (!OnMoveCameraRightLeft.ExecuteIfBound(Value.Get<float>()))
    {
        UE_LOG(LogCameraController, Error, TEXT("OnMoveCameraRightLeft Delegate is not bound"));
        checkNoEntry();
    }
}

void ATDCameraController::ZoomUpAction(const FInputActionValue& Value)
{
    if (!OnZoomChanged.ExecuteIfBound(Value.Get<float>()))
    {
        UE_LOG(LogCameraController, Error, TEXT("OnZoomChanged Delegate is not bound"));
        checkNoEntry();
    }
}

void ATDCameraController::RotateCamera(const FInputActionValue& Value)
{
    if (!OnRotateCamera.ExecuteIfBound(Value.Get<float>()))
    {
        UE_LOG(LogCameraController, Error, TEXT("OnRotateCamera Delegate is not bound"));
        checkNoEntry();
    }
}

void ATDCameraController::ChangeAngleCamera(const FInputActionValue& Value)
{
    if (!OnChangeAngleCamera.ExecuteIfBound(Value.Get<float>()))
    {
        UE_LOG(LogCameraController, Error, TEXT("OnChangeAngleCamera Delegate is not bound"));
        checkNoEntry();
    }
}

void ATDCameraController::StopSquad()
{
    if (!OnStopSquad.ExecuteIfBound())
    {
        UE_LOG(LogCameraController, Error, TEXT("OnStopSquad Delegate is not bound"));
        checkNoEntry();
    }
}

void ATDCameraController::GroupingSquad()
{
    if (!OnGroupingSquad.ExecuteIfBound())
    {
        UE_LOG(LogCameraController, Error, TEXT("OnGroupingSquad Delegate is not bound"));
        checkNoEntry();
    }
}

void ATDCameraController::MultiplySelectSquadsOn()
{
    if (!OnMultiplySelectSquad.ExecuteIfBound(true))
    {
        UE_LOG(LogCameraController, Error, TEXT("OnMultiplySelectSquad Delegate is not bound"));
        checkNoEntry();
    }
}

void ATDCameraController::MultiplySelectSquadsOff()
{
    if (!OnMultiplySelectSquad.ExecuteIfBound(false))
    {
        UE_LOG(LogCameraController, Error, TEXT("OnMultiplySelectSquad Delegate is not bound"));
        checkNoEntry();
    }
}

void ATDCameraController::LeftClickStarted()
{
    if (!OnLeftMouseClickChois.ExecuteIfBound(GetClickHit()))
    {
        UE_LOG(LogCameraController, Error, TEXT("OnLeftMouseClickChois Delegate is not bound"));
        checkNoEntry();
    }
}

void ATDCameraController::LeftClickTriggered()
{
    LeftMouseDownTime = GetInputKeyTimeDown(EKeys::LeftMouseButton);

    if (LeftMouseDownTime > LeftMouseButtonHoldTreshold && !bSelectionBoxIsSpawned)
    {
        if (OnLeftMouseHold.ExecuteIfBound())
        {
            bSelectionBoxIsSpawned = true;
        }
        else
        {
            UE_LOG(LogCameraController, Error, TEXT("OnLeftMouseHold Delegate is not bound"));
            checkNoEntry();
        }
    }
}

void ATDCameraController::LeftClickCompleted()
{
    if (LeftMouseDownTime > LeftMouseButtonHoldTreshold)
    {
        if (OnLeftMouseHoldCompleted.ExecuteIfBound())
        {
            LeftMouseDownTime      = 0.0f;
            bSelectionBoxIsSpawned = false;
        }
        else
        {
            UE_LOG(LogCameraController, Error, TEXT("OnLeftMouseHold Delegate is not bound"));
            checkNoEntry();
        }
    }
}

void ATDCameraController::RightClickTriggered()
{
    RightMouseDownTime = GetInputKeyTimeDown(EKeys::RightMouseButton);

    if (RightMouseDownTime >= RightMouseButtonHoldTreshold && !bRebuildSquadIsContinue)
    {
        if (OnRightMouseHold.ExecuteIfBound())
        {
            bRebuildSquadIsContinue = true;
        }
        else
        {
            UE_LOG(LogCameraController, Error, TEXT("OnRightMouseHold Delegate is not bound"));
            checkNoEntry();
        }
    }
}

void ATDCameraController::RightClickCompleted() 
{
    if (RightMouseDownTime > RightMouseButtonHoldTreshold)
    {
        if (OnRightMouseHoldCompleted.ExecuteIfBound())
        {
            RightMouseDownTime      = 0.0f;
            bRebuildSquadIsContinue = false;
        }
        else
        {
            UE_LOG(LogCameraController, Error, TEXT("OnRightMouseHoldCompleted Delegate is not bound"));
            checkNoEntry();
        }
    }
    else
    {
        if (!OnRightMouseClickChois.ExecuteIfBound(GetClickHit()))
        {
            UE_LOG(LogCameraController, Error, TEXT("OnRightMouseClickChois Delegate is not bound"));
            checkNoEntry();
        }
    }
}

FHitResult ATDCameraController::GetClickHit()
{
    FHitResult Hit{};
    FVector MouseWorldLocation{};
    FVector MouseWorldDirection{};

    bool bHitSuccessful = DeprojectMousePositionToWorld(MouseWorldLocation, MouseWorldDirection);
    if (!bHitSuccessful)
    {
        UE_LOG(LogCameraController, Error, TEXT("Unable to determine value Mouse Click"));
        checkNoEntry();
    }

    FVector TraceEnd = MouseWorldLocation + MouseWorldDirection * 100000;

    bHitSuccessful = GetWorld()->LineTraceSingleByChannel(Hit, MouseWorldLocation, TraceEnd, ECollisionChannel::ECC_Visibility);
    if (!bHitSuccessful)
    {
        UE_LOG(LogCameraController, Error, TEXT("Trace no Hit on Click"));
    }
    return Hit;
}

FVector ATDCameraController::GetMouseLocationOnTerrain()
{
    FHitResult Hit;
    FVector MouseWorldLocation{};
    FVector MouseWorldDirection{};
    if (DeprojectMousePositionToWorld(MouseWorldLocation, MouseWorldDirection))
    {
        const float TraceDistance = 1000000.f;
        FVector TraceEnd          = MouseWorldLocation + MouseWorldDirection * TraceDistance;

        GetWorld()->LineTraceSingleByChannel(Hit, MouseWorldLocation, TraceEnd, ECollisionChannel::ECC_GameTraceChannel1);

        return Hit.Location;
    }
    else
    {
        return FVector::Zero();
    }
}
