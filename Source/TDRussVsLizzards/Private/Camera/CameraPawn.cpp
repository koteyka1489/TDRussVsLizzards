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
    }

    GetSquadsOnLevel();
    BindOnSquadIsChoisenDelegate();
}

void ACameraPawn::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
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
    SetActorLocation(GetActorLocation() + GetActorForwardVector() * Direction * SpeedCamera * DeltaTime);
}

void ACameraPawn::OnMoveCameraRightLeft(float Direction)
{
    float DeltaTime = UGameplayStatics::GetWorldDeltaSeconds(GetWorld());
    SetActorLocation(GetActorLocation() + GetActorRightVector() * Direction * SpeedCamera * DeltaTime);
}

void ACameraPawn::OnRotateCamera(float Direction)
{
    float DeltaTime    = UGameplayStatics::GetWorldDeltaSeconds(GetWorld());
    FRotator NewRotate = GetActorRotation() + FRotator(0.0f, Direction * SpeedRotateCamera * DeltaTime, 0.0f);
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
            ChoisenSquads.Empty();
        }
    }
}

void ACameraPawn::OnSquadIsChoisen(ABaseSquadCreeps* SquadIn)
{
    for (auto Squad : ChoisenSquads)
    {
        if (Squad.Get() == SquadIn) return;
    }

    ChoisenSquads.Add(SquadIn);
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
