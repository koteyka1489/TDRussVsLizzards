// TD Russ Vs Lizzards Game

#include "Camera/CameraPawn.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/TDCameraController.h"
#include "Kismet/GameplayStatics.h"
#include "Creeps/BaseCreepActor.h"
#include "Squad/BaseSquadCreeps.h"
#include "Kismet/GameplayStatics.h"

ACameraPawn::ACameraPawn()
{
    PrimaryActorTick.bCanEverTick = true;

    SceneComponent = CreateDefaultSubobject<USceneComponent>("SceneComponent");
    SetRootComponent(SceneComponent);

    SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>("SpringArmComponent");
    SpringArmComponent->SetupAttachment(GetRootComponent());

    CameraComponent = CreateDefaultSubobject<UCameraComponent>("CameraComponent");
    CameraComponent->SetupAttachment(SpringArmComponent);

    SpringArmComponent->TargetArmLength = ZoomStart;
}

void ACameraPawn::BeginPlay()
{
    Super::BeginPlay();

    auto CameraController = Cast<ATDCameraController>(GetController());
    if (CameraController)
    {
        CameraController->OnZoomChanged.BindUObject(this, &ACameraPawn::OnZoomChanged);
        CameraController->OnMoveCameraUpDown.BindUObject(this, &ACameraPawn::OnMoveCameraUpDown);
        CameraController->OnMoveCameraRightLeft.BindUObject(this, &ACameraPawn::OnMoveCameraRightLeft);
        CameraController->OnRotateCamera.BindUObject(this, &ACameraPawn::OnRotateCamera);
        CameraController->OnLeftMouseClickChois.BindUObject(this, &ACameraPawn::OnLeftMouseClickChois);
        CameraController->OnRightMouseClickChois.BindUObject(this, &ACameraPawn::OnRightMouseClickChois);
        CameraController->OnChangeAngleCamera.BindUObject(this, &ACameraPawn::OnChangeAngleCamera);
        CameraController->OnMultiplySelectSquad.BindUObject(this, &ACameraPawn::OnMultiplySelectSquad);
    }

    GetSquadsOnLevel();
    BindOnSquadIsChoisenDelegate();
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

void ACameraPawn::OnLeftMouseClickChois(FHitResult Hit)
{
    auto Creep = Cast<ABaseCreepActor>(Hit.GetActor());
    if (IsValid(Creep))
    {
        Creep->SetCreepIsClicked();
    }
    else
    {
        UnchoiseCurrentSquad();
    }
}

void ACameraPawn::OnRightMouseClickChois(FHitResult Hit)
{
    if (ChoisenSquads.Num() == 0) return;

    for (auto& Squad : ChoisenSquads)
    {
        Squad->MoveToLocation(Hit.Location);
    }
}

void ACameraPawn::OnSquadIsChoisen(ABaseSquadCreeps* SquadIn)
{
    if (bMultiplySelectSquad)
    {
        for (auto Squad : ChoisenSquads)
        {
            if (Squad.Get() == SquadIn) return;
        }
        ChoisenSquads.Add(SquadIn);
    }
    else
    {
        UnchoiseCurrentSquad();
        ChoisenSquads.Add(SquadIn);
    }
}

void ACameraPawn::OnMultiplySelectSquad(bool Value)
{
    bMultiplySelectSquad = Value;
}

void ACameraPawn::GetSquadsOnLevel()
{
    TArray<AActor*> Actors;
    UGameplayStatics::GetAllActorsOfClass(GetWorld(), ABaseSquadCreeps::StaticClass(), Actors);

    checkf(!Actors.IsEmpty(), TEXT("Get All Squads on level is Failed"));

    for (auto& Actor : Actors)
    {
        auto Squad = Cast<ABaseSquadCreeps>(Actor);
        if (IsValid(Squad))
        {
            SquadsOnLevel.Add(Squad);
        }
    }
}

void ACameraPawn::BindOnSquadIsChoisenDelegate()
{
    checkf(!SquadsOnLevel.IsEmpty(), TEXT("SquadsOnLevel is Empty"));

    for (auto& Squad : SquadsOnLevel)
    {
        Squad->OnSquadIsChoisen.BindUObject(this, &ACameraPawn::OnSquadIsChoisen);
    }
}

void ACameraPawn::UnchoiseCurrentSquad()
{
    if (ChoisenSquads.Num() == 0)
    {
        return;
    }
    else
    {
        for (auto& Squad : ChoisenSquads)
        {
            Squad->SquadUnChoisen();
        }
        ChoisenSquads.Empty(20);
    }
}

void ACameraPawn::MoveCameraByMouse()
{
    auto CameraController = Cast<ATDCameraController>(GetController());
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
