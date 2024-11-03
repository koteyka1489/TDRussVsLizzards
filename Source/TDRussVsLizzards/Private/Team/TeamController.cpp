// TD Russ Vs Lizzards Game

#include "Team/TeamController.h"
#include "Squad/BaseSquadCreeps.h"
#include "Camera/TDCameraController.h"
#include "Kismet/GameplayStatics.h"
#include "Components/BoxComponent.h"
#include "Camera/SelectionBox.h"

#include "GameFramework/GameStateBase.h"

ATeamController::ATeamController()
{
    PrimaryActorTick.bCanEverTick = true;
}

void ATeamController::BeginPlay()
{
    Super::BeginPlay();

    CameraController = Cast<ATDCameraController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
    if (CameraController)
    {
        CameraController->OnLeftMouseClickChois.BindUObject(this, &ATeamController::OnLeftMouseClickChois);
        CameraController->OnRightMouseClickChois.BindUObject(this, &ATeamController::OnRightMouseClick);
        CameraController->OnMultiplySelectSquad.BindUObject(this, &ATeamController::OnMultiplySelectSquad);
        CameraController->OnLeftMouseHold.BindUObject(this, &ATeamController::OnLeftMouseHold);
        CameraController->OnLeftMouseHoldCompleted.BindUObject(this, &ATeamController::OnLeftMouseHoldCompleted);
        CameraController->OnStopSquad.BindUObject(this, &ATeamController::OnStopSquad);
        CameraController->OnRightMouseHold.BindUObject(this, &ATeamController::OnRightMouseHold);
        CameraController->OnRightMouseHoldCompleted.BindUObject(this, &ATeamController::OnRightMouseHoldCompleted);
    }

    GetSquadsOnLevel();
    BindOnSquadIsChoisenDelegate();
}

void ATeamController::Tick(float DeltaTime)
{
    if (bBoxIsSpawned)
    {
        SelectionBox->Update(CameraController->GetMouseLocationOnTerrain());
    }
}

void ATeamController::SetSquadIsChoisen(TObjectPtr<ABaseSquadCreeps> ChoisenSquad)
{
    ChoisenSquad->SetSquadIsChoisen();
    AddSquadToChoisenSquadsArray(ChoisenSquad);
}

void ATeamController::OnLeftMouseClickChois(FHitResult Hit)
{
    auto Box = Cast<UBoxComponent>(Hit.GetComponent());
    if (!Box)
    {
        UnchoiseCurrentSquad();
        return;
    }

    auto Squad = Cast<ABaseSquadCreeps>(Box->GetOwner());

    if (!Squad)
    {
        UnchoiseCurrentSquad();
        return;
    }

    SetSquadIsChoisen(Squad);
}

void ATeamController::OnRightMouseClick(FHitResult Hit)
{
    GEngine->AddOnScreenDebugMessage(-1, 3, FColor::Red, TEXT("Right Mouse Click"));
    if (ChoisenSquads.Num() == 0) return;

    for (auto& Squad : ChoisenSquads)
    {
        Squad->MoveAndRotatingSquadToLocation(Hit.Location);
    }
}

void ATeamController::AddSquadToChoisenSquadsArray(ABaseSquadCreeps* SquadIn)
{
    if (bMultiplySelectSquadByClick || bMultiplySelectSquadBySelectedBox)
    {
        for (auto& Squad : ChoisenSquads)
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

void ATeamController::OnSquadIsUnChoisen(ABaseSquadCreeps* SquadIn)
{
    ChoisenSquads.RemoveSingle(SquadIn);
}

void ATeamController::OnMultiplySelectSquad(bool Value)
{
    bMultiplySelectSquadByClick = Value;
}

void ATeamController::OnLeftMouseHold()
{
    if (!bBoxIsSpawned)
    {
        CreateSelectionBox();
        bBoxIsSpawned                     = true;
        bMultiplySelectSquadBySelectedBox = true;
    }
}

void ATeamController::OnLeftMouseHoldCompleted()
{
    if (bBoxIsSpawned)
    {
        SelectionBox->SelectionComplete();
        SelectionBox                      = nullptr;
        bBoxIsSpawned                     = false;
        bMultiplySelectSquadBySelectedBox = false;
    }
}

void ATeamController::OnRightMouseHold() 
{
    GEngine->AddOnScreenDebugMessage(-1, 3, FColor::Red, TEXT("Right Mouse hold"));
}

void ATeamController::OnRightMouseHoldCompleted()
{
    GEngine->AddOnScreenDebugMessage(-1, 3, FColor::Red, TEXT("Right Mouse Complete"));
}

void ATeamController::OnStopSquad()
{
    if (ChoisenSquads.Num() == 0) return;

    for (auto& Squad : ChoisenSquads)
    {
        Squad->StopAllTasks();
    }
}

void ATeamController::GetSquadsOnLevel()
{
    TArray<AActor*> Actors;
    UGameplayStatics::GetAllActorsOfClass(GetWorld(), ABaseSquadCreeps::StaticClass(), Actors);

    if (Actors.IsEmpty()) return;

    for (auto& Actor : Actors)
    {
        auto Squad = Cast<ABaseSquadCreeps>(Actor);
        if (IsValid(Squad))
        {
            SquadsOnLevel.Add(Squad);
        }
    }
}

void ATeamController::BindOnSquadIsChoisenDelegate()
{
    if (SquadsOnLevel.IsEmpty()) return;

    for (auto& Squad : SquadsOnLevel)
    {
        Squad->OnSquadIsUnChoisenBySelectionBox.BindUObject(this, &ATeamController::OnSquadIsUnChoisen);
    }
}

void ATeamController::UnchoiseCurrentSquad()
{
    if (ChoisenSquads.Num() == 0) return;

    for (auto& Squad : ChoisenSquads)
    {
        Squad->SquadUnChoisen();
    }
    ChoisenSquads.Empty(20);
}

void ATeamController::CreateSelectionBox()
{
    if (!GetWorld()) return;

    FActorSpawnParameters SpawnInfo;
    SpawnInfo.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
    SpawnInfo.Owner                          = this;
    SelectionBoxStartLocation                = CameraController->GetMouseLocationOnTerrain();

    SelectionBox =
        GetWorld()->SpawnActor<ASelectionBox>(SelectionBoxDefaultClass, SelectionBoxStartLocation, FRotator::ZeroRotator, SpawnInfo);
}