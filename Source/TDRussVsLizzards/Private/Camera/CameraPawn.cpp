// TD Russ Vs Lizzards Game

#include "Camera/CameraPawn.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/TDCameraController.h"
#include "Kismet/GameplayStatics.h"
#include "Squad/BaseSquadCreeps.h"
#include "Kismet/GameplayStatics.h"
#include "Camera/SelectionBox.h"
#include "Components/BoxComponent.h"

ACameraPawn::ACameraPawn()
{
    PrimaryActorTick.bCanEverTick = true;

    SceneComponent = CreateDefaultSubobject<USceneComponent>("SceneComponent");
    SetRootComponent(SceneComponent);

    SpringArmComponent                  = CreateDefaultSubobject<USpringArmComponent>("SpringArmComponent");
    SpringArmComponent->TargetArmLength = ZoomStart;
    SpringArmComponent->SetupAttachment(GetRootComponent());

    CameraComponent = CreateDefaultSubobject<UCameraComponent>("CameraComponent");
    CameraComponent->SetupAttachment(SpringArmComponent);
}

void ACameraPawn::BeginPlay()
{
    Super::BeginPlay();

    CameraController = Cast<ATDCameraController>(GetController());
    if (CameraController)
    {
        CameraController->OnZoomChanged.BindUObject(this, &ACameraPawn::OnZoomChanged);
        CameraController->OnMoveCameraUpDown.BindUObject(this, &ACameraPawn::OnMoveCameraUpDown);
        CameraController->OnMoveCameraRightLeft.BindUObject(this, &ACameraPawn::OnMoveCameraRightLeft);
        CameraController->OnRotateCamera.BindUObject(this, &ACameraPawn::OnRotateCamera);
        CameraController->OnChangeAngleCamera.BindUObject(this, &ACameraPawn::OnChangeAngleCamera);
    }
}

void ACameraPawn::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    MoveCameraByMouse();
}

void ACameraPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void ACameraPawn::OnZoomChanged(float Direction)
{
    float DeltaTime                     = UGameplayStatics::GetWorldDeltaSeconds(GetWorld());
    float ArmLengthOffset               = SpeedZoom * Direction * DeltaTime;
    SpringArmComponent->TargetArmLength = FMath::Clamp(SpringArmComponent->TargetArmLength += ArmLengthOffset, ZoomMin, ZoomMax);
}

void ACameraPawn::OnMoveCameraUpDown(float Direction)
{
    float DeltaTime = UGameplayStatics::GetWorldDeltaSeconds(GetWorld());
    double Pitch    = GetActorRotation().Pitch;

    FQuat QRot                    = FQuat(GetActorRightVector(), FMath::DegreesToRadians(Pitch));
    FVector AdjustedForwardVector = QRot.RotateVector(GetActorForwardVector());

    SetActorLocation(GetActorLocation() + AdjustedForwardVector * Direction * SpeedCamera * DeltaTime);
}

void ACameraPawn::OnMoveCameraRightLeft(float Direction)
{
    float DeltaTime = UGameplayStatics::GetWorldDeltaSeconds(GetWorld());
    SetActorLocation(GetActorLocation() + GetActorRightVector() * Direction * SpeedCamera * DeltaTime);
}

void ACameraPawn::OnRotateCamera(float Direction)
{
    float DeltaTime    = UGameplayStatics::GetWorldDeltaSeconds(GetWorld());
    FRotator NewRotate = GetActorRotation() + FRotator(0.0, Direction * SpeedRotateCamera * DeltaTime, 0.0);
    SetActorRotation(NewRotate);
}

void ACameraPawn::OnChangeAngleCamera(float Direction)
{

    double DeltaTime        = UGameplayStatics::GetWorldDeltaSeconds(GetWorld());
    FRotator CameraRotation = GetActorRotation();

    double TargetPitch =
        FMath::Clamp(CameraRotation.Pitch + Direction * SpeedRotatePitchCamera * DeltaTime, MinPitchCamera, MaxPitchCamera);

    FRotator NewRotate = FRotator(TargetPitch, CameraRotation.Yaw, CameraRotation.Roll);
    SetActorRotation(NewRotate);
}

void ACameraPawn::MoveCameraByMouse()
{

    if (CameraController)
    {
        int32 XScreenSize;
        int32 YScreenSize;
        CameraController->GetViewportSize(XScreenSize, YScreenSize);

        double XMousePos;
        double YMousePos;
        CameraController->GetMousePosition(XMousePos, YMousePos);

        if ((int32)XMousePos <= MoveMouseTreshold && (int32)XMousePos > 0)
        {
            OnMoveCameraRightLeft(-1.0f);
        }
        else if ((int32)XMousePos <= XScreenSize && (int32)XMousePos >= (XScreenSize - MoveMouseTreshold))
        {
            OnMoveCameraRightLeft(1.0f);
        }

        if ((int32)YMousePos <= MoveMouseTreshold && (int32)YMousePos > 0)
        {
            OnMoveCameraUpDown(1.0f);
        }
        else if ((int32)YMousePos <= YScreenSize && (int32)YMousePos >= (YScreenSize - MoveMouseTreshold))
        {
            OnMoveCameraUpDown(-1.0f);
        }
    }
}
