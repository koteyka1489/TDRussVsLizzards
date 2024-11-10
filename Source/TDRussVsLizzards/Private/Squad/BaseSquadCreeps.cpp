// TD Russ Vs Lizzards Game

#include "Squad/BaseSquadCreeps.h"
#include "Creeps/BaseCreepActor.h"
#include "Creeps/OrcCreepActor.h"
#include "Creeps/TrollCreepActor.h"
#include "Creeps/BarbarianCreepActor.h"
#include "DrawDebugHelpers.h"
#include "Components/SceneComponent.h"
#include "Components/BoxComponent.h"
#include "Squad/Task/MoveSquadTask.h"
#include "Squad/Task/RotateCreepsTask.h"
#include "Squad/Task/RotateFrontSquadTask.h"
#include "Squad/Task/SquadBaseTask.h"
#include "Camera/CameraPawn.h"
#include "Kismet/GameplayStatics.h"
#include "Team/TeamController.h"
#include "Components/InstancedStaticMeshComponent.h"
#include "Components/SquadCalcMovementTargetComponent.h"
#include "Components/SquadMovementComponent.h"

ABaseSquadCreeps::ABaseSquadCreeps()
{
    PrimaryActorTick.bCanEverTick = true;

    SceneComponent = CreateDefaultSubobject<USceneComponent>("SceneComponent");
    SetRootComponent(SceneComponent);

    SquadSizesBox = CreateDefaultSubobject<UBoxComponent>("BoxComponent");
    SquadSizesBox->SetBoxExtent(FVector(1.0, 1.0, 1.0));
    SquadSizesBox->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
    SquadSizesBox->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);
    SquadSizesBox->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility, ECollisionResponse::ECR_Block);
    SquadSizesBox->SetVisibility(true);
    SquadSizesBox->bHiddenInGame = false;
    SquadSizesBox->SetupAttachment(GetRootComponent());

    SquadMovementComponent   = CreateDefaultSubobject<USquadMovementComponent>("USquadMovementComponent");
    SquadCalcTargetComponent = CreateDefaultSubobject<USquadCalcMovementTargetComponent>("USquadCalcMovementTargetComponent");

    InstancedNewLocationMesh                    = CreateDefaultSubobject<UInstancedStaticMeshComponent>("InstancedNewLocationMesh");
    InstancedNewLocationMesh->bDisableCollision = true;
    InstancedNewLocationMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    InstancedNewLocationMesh->SetCollisionResponseToAllChannels(ECR_Ignore);
    InstancedNewLocationMesh->SetGenerateOverlapEvents(false);
    check(IsValid(InstancedNewLocationMesh));
    static ConstructorHelpers::FObjectFinder<UStaticMesh> SelectCircleMesh(
        TEXT("/Script/Engine.StaticMesh'/Game/Squads/SM_SelectCircle.SM_SelectCircle'"));
    checkf(SelectCircleMesh.Succeeded(), TEXT("Find SelectCircleMesh is not Succeeded "));
    InstancedNewLocationMesh->SetStaticMesh(SelectCircleMesh.Object);
}

void ABaseSquadCreeps::BeginPlay()
{
    Super::BeginPlay();

    Creeps.Reserve(CreepsNum);
    SpawnCreeps();
    UpdateSquadLocationStart();
    SetBoxExtendBySquadSize();

    auto TeamController = Cast<ATeamController>(UGameplayStatics::GetActorOfClass(GetWorld(), ATeamController::StaticClass()));
    if (TeamController)
    {
        SetOwner(TeamController);
    }

    for (auto& Creep : Creeps)
    {
        float RotatingMove = CreepsSpeed.SpeedRotating + FMath::RandRange(-CreepsSpeed.RotatingRandom, CreepsSpeed.RotatingRandom);
        float SpeedMove    = CreepsSpeed.SpeedMoving + FMath::RandRange(-CreepsSpeed.MovingRandom, CreepsSpeed.MovingRandom);
        Creep->SetCreepMaxSpeeds(RotatingMove, SpeedMove);
    }
}

void ABaseSquadCreeps::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    DrawDebugLine(GetWorld(), GetActorLocation(), GetActorLocation() + GetActorForwardVector() * 1000, FColor::Red, false, -1.f, 0u, 20.0f);

    ExecuteCurrentTaskQueue();
}

void ABaseSquadCreeps::UpdateSquadLocationStart()
{
    FVector SquadRightCorner              = GetRightCornerCreepLocation();
    FVector SquadLeftCorner               = GetLeftCornerCreepLocation();
    FVector SquadRightToLeftCornerHalfVec = (SquadLeftCorner - SquadRightCorner) / 2;

    int32 IndexCreepBackRightCorner = (CurrentSquadSizes.Heigth - 1) * CurrentSquadSizes.Width;
    checkf(Creeps.IsValidIndex(IndexCreepBackRightCorner), TEXT("Invalid Index"));

    FVector BackCorner         = Creeps[IndexCreepBackRightCorner]->GetActorLocation();
    FVector FrontToBackHalfVec = (BackCorner - SquadRightCorner) / 2;

    // Set Center squad on center Spawned creeps
    SetActorLocation(GetActorLocation() + SquadRightToLeftCornerHalfVec + FrontToBackHalfVec);
}

void ABaseSquadCreeps::SetBoxExtendBySquadSize()
{
    FVector RightCornerFrontNewPos = Creeps[0]->GetActorLocation();

    FVector LeftCornerForntNewPos = Creeps[CurrentSquadSizes.Width - 1]->GetActorLocation();
    FVector RightBackCorner       = Creeps[(CurrentSquadSizes.Heigth - 1) * CurrentSquadSizes.Width]->GetActorLocation();

    double NewWidth  = (LeftCornerForntNewPos - RightCornerFrontNewPos).Size() / 2;
    double NewHeight = (RightBackCorner - RightCornerFrontNewPos).Size() / 2;

    FVector NewBoxExtend{NewHeight, NewWidth, 200.0};

    SquadSizesBox->SetBoxExtent(NewBoxExtend);
}

void ABaseSquadCreeps::SpawnCreeps()
{
    CurrentSquadSizes.Heigth = 6;
    CurrentSquadSizes.Width  = CreepsNum / CurrentSquadSizes.Heigth;

    int32 CreepsShortage = CreepsNum - CurrentSquadSizes.Heigth * CurrentSquadSizes.Width;

    const FVector Squadlocation = GetActorLocation();
    FRotator SpawnRotation{0.0, 0.0, 0.0};
    FActorSpawnParameters SpawnInfo;
    SpawnInfo.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
    SpawnInfo.Owner                          = this;

    TArray<FVector> SpawnLocations =
        CalculateCreepsPositions(0, CurrentSquadSizes.Heigth, 0, CurrentSquadSizes.Width, Squadlocation, SquadBaseForwardVector);

    int32 StartSpawnRemainderCreeps = CurrentSquadSizes.Width / 2 - CreepsShortage / 2;
    SpawnLocations.Append(CalculateCreepsPositions(CurrentSquadSizes.Heigth, CurrentSquadSizes.Heigth + 1, StartSpawnRemainderCreeps,
        StartSpawnRemainderCreeps + CreepsShortage, Squadlocation, SquadBaseForwardVector));

    for (const auto& SpawnLocation : SpawnLocations)
    {
        ABaseCreepActor* SpawnedCreep = GetWorld()->SpawnActor<ABaseCreepActor>(CreepsType, SpawnLocation, SpawnRotation, SpawnInfo);
        Creeps.Add(SpawnedCreep);
    }
}

TArray<FVector> ABaseSquadCreeps::CalculateCreepsPositions(int32 HeightStart, int32 HeightEnd, int32 WidthStart, int32 WidthEnd,
    FVector SquadBaseSpawnLocation, FVector ForwarVectorToNewLocation, bool UseLocationRandom)
{
    TArray<FVector> Result;

    for (int32 HeightPos = HeightStart; HeightPos < HeightEnd; HeightPos++)
    {
        for (int32 WidthPos = WidthStart; WidthPos < WidthEnd; WidthPos++)
        {
            double XLocRand = UseLocationRandom ? FMath::FRandRange(-CreepPositionRandom, CreepPositionRandom) : 0.0;
            double YLocRand = UseLocationRandom ? FMath::FRandRange(-CreepPositionRandom, CreepPositionRandom) : 0.0;

            FVector VecFromBaseSpawnLoc(
                XLocRand - CreepsOffsetInSquad.X * (double)HeightPos, YLocRand - CreepsOffsetInSquad.Y * (double)WidthPos, 0.0);

            FVector VecFromBaseSpawnLocRotated =
                CalculateQuatBeetwenBaseSquadVec(ForwarVectorToNewLocation).RotateVector(VecFromBaseSpawnLoc);

            const FVector SpawnLocation =
                FVector(SquadBaseSpawnLocation.X, SquadBaseSpawnLocation.Y, SquadBaseSpawnLocation.Z + 90.0) + VecFromBaseSpawnLocRotated;

            Result.Add(SpawnLocation);
        }
    }

    return Result;
}

FQuat ABaseSquadCreeps::CalculateQuatBeetwenBaseSquadVec(FVector VectorIn)
{
    FQuat Result = FQuat::FindBetweenVectors(SquadBaseForwardVector, VectorIn);

    return Result.GetNormalized();
}

void ABaseSquadCreeps::UpdateRebuildngSquad(int32 NewWidth, FVector NewStartCreepSpawnLocation, FVector NewSquadForwardVerctor)
{
    SquadCalcTargetComponent->UpdateNewCreepsPositions(NewWidth, NewStartCreepSpawnLocation, NewSquadForwardVerctor);
    UpdateInstancedNewLocationMesh(SquadCalcTargetComponent->GetNewCreepsLocations(), SquadCalcTargetComponent->GetNewSquadRotation());
}

void ABaseSquadCreeps::EndRebuildSquad()
{
    DeleteInstancedNewLocationMesh();
    SquadCalcTargetComponent->SetCreepsMovingDestination();
    SquadMovementComponent->SetSquadMovement();
}

void ABaseSquadCreeps::SetSquadIsChoisen()
{
    if (bSquadIsChosen) return;

    bSquadIsChosen = true;

    for (auto& Creep : Creeps)
    {
        Creep->SetCreepIsChoisen(true);
    }
}

FVector ABaseSquadCreeps::GetRightCornerCreepLocation()
{
    return Creeps[0]->GetActorLocation();
}

FVector ABaseSquadCreeps::GetLeftCornerCreepLocation()
{
    return Creeps[CurrentSquadSizes.Width - 1]->GetActorLocation();
}

void ABaseSquadCreeps::OnRebuildingSquadComplete()
{
    FVector VectorRebuildRotation = GetActorLocation() + RebuildSquadNewForwardVector * 1000;
    auto RotateCreepsToDestTask   = NewObject<URotateCreepsTask>();
    RotateCreepsToDestTask->InitDestinationTask(VectorRebuildRotation, this);
    SquadTasksQueue.Enqueue(RotateCreepsToDestTask);
    SetBoxExtendBySquadSize();

    FVector VectorRebuildMovement = GetActorLocation() + RebuildSquadNewForwardVector;
    auto MoveToLocationTask       = NewObject<UMoveSquadTask>();
    MoveToLocationTask->InitDestinationTask(VectorRebuildMovement, this);
    SquadTasksQueue.Enqueue(MoveToLocationTask);
}

void ABaseSquadCreeps::SquadUnChoisen()
{
    if (!bSquadIsChosen) return;

    bSquadIsChosen = false;

    for (auto& Creep : Creeps)
    {
        Creep->SetCreepIsChoisen(false);
    }
}

void ABaseSquadCreeps::SquadUnChoisenBySelectBox()
{
    if (!bSquadIsChosen) return;

    bSquadIsChosen = false;

    if (OnSquadIsUnChoisenBySelectionBox.ExecuteIfBound(this))
    {
        for (auto& Creep : Creeps)
        {
            Creep->SetCreepIsChoisen(false);
        }
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("OnSquadIsUnChoisenBySelectionBox Is not bound"));
        checkNoEntry();
    }
}

void ABaseSquadCreeps::MoveAndRotatingSquadToLocation(FVector Destination)
{
    FVector NewRightCorner = CalculateNewRightCorner();
    
    SquadCalcTargetComponent->UpdateNewCreepsPositions(
        CurrentSquadSizes.Width, NewRightCorner, (Destination - GetActorLocation()).GetSafeNormal2D());
    SquadCalcTargetComponent->SetCreepsMovingDestination();
    SquadMovementComponent->SetSquadMovement();
}

void ABaseSquadCreeps::StopAllTasks()
{
    SquadMovementComponent->StopAllMovings();
    CurrentSquadTask           = nullptr;
    bCurrentSquadTaskIsExecute = false;
    SquadTasksQueue.Empty();
}

double ABaseSquadCreeps::CalculateDotFrontSquadToLocation(FVector Location)
{
    FVector SquadForwardVector             = GetActorForwardVector();
    FVector SquadToLocationNormalizeVector = (Location - GetActorLocation()).GetSafeNormal2D();
    return SquadForwardVector.Dot(SquadToLocationNormalizeVector);
}

double ABaseSquadCreeps::CalculateDotRightVectorSquadToLocation(FVector Location)
{
    FVector SquadRightVector               = GetActorRightVector();
    FVector SquadToLocationNormalizeVector = (Location - GetActorLocation()).GetSafeNormal2D();
    return SquadRightVector.Dot(SquadToLocationNormalizeVector);
}



void ABaseSquadCreeps::ExecuteCurrentTaskQueue()
{

    TObjectPtr<USquadBaseTask> FirstQueueTask;
    if (!SquadTasksQueue.Peek(FirstQueueTask) || bCurrentSquadTaskIsExecute || SquadTasksQueue.IsEmpty()) return;

    if (CurrentSquadTask == FirstQueueTask)
    {
        return;
    }
    else
    {
        TObjectPtr<USquadBaseTask> NewCurrenWueueTask;
        if (SquadTasksQueue.Dequeue(NewCurrenWueueTask))
        {
            bCurrentSquadTaskIsExecute = true;
            CurrentSquadTask           = NewCurrenWueueTask;
            CurrentSquadTask->ExecuteTask();
        }
    }
}

FVector ABaseSquadCreeps::CalculateNewSquadCenterOnRebuild()
{
    FVector RightCornerFrontNewPos = RebuildCreepsNewLocations[0];
    FVector LeftCornerForntNewPos  = RebuildCreepsNewLocations[CurrentSquadSizes.Width - 1];
    FVector RightBackCorner        = RebuildCreepsNewLocations[(CurrentSquadSizes.Heigth - 1) * CurrentSquadSizes.Width];

    FVector RightToLeftInterp = (LeftCornerForntNewPos - RightCornerFrontNewPos) / 2;
    FVector FrontToBackInterp = (RightBackCorner - RightCornerFrontNewPos) / 2;

    return RebuildCreepsNewLocations[0] + RightToLeftInterp + FrontToBackInterp;
}

FVector ABaseSquadCreeps::CalculateNewRightCorner()
{

    // need calculate new right cornenr suqad on moving
    return FVector();
}

void ABaseSquadCreeps::UpdateInstancedNewLocationMesh(const TArray<FVector>& NewPositions, const FRotator& NewSquadRotation)
{
    if (!InstancedMeshNewLocIsSet)
    {
        InstancedNewLocationMesh->SetVisibility(true);
        InstancedNewLocationMesh->bHiddenInGame = false;

        for (const auto& CreepLocation : NewPositions)
        {
            FTransform NewTransform(NewSquadRotation, CreepLocation, FVector(1.0, 1.0, 1.0));
            InstancedNewLocationMesh->AddInstance(NewTransform);
        }
        InstancedMeshNewLocIsSet = true;
    }
    else
    {
        int32 IndexInstancedMesh = 0;
        for (const auto& CreepLocation : NewPositions)
        {
            FTransform NewTransform(NewSquadRotation, CreepLocation, FVector(1.0, 1.0, 1.0));
            InstancedNewLocationMesh->UpdateInstanceTransform(IndexInstancedMesh, NewTransform, true, true, true);
            IndexInstancedMesh++;
        }
    }
}

void ABaseSquadCreeps::DeleteInstancedNewLocationMesh()
{
    InstancedNewLocationMesh->SetVisibility(false);
    InstancedNewLocationMesh->bHiddenInGame = true;
    InstancedNewLocationMesh->ClearInstances();
    InstancedMeshNewLocIsSet = false;
}
